let usb_enabled = false /** USB enabled status. */;
let baudrate = 115200; /** baud rate. */
let databits = 8; /** number of data bits. */
let stopbits = 1; /** number of stop bits. */
let parity = 0; /** type of parity. */
let dtr = true; /** data terminal ready signal. */
let rts = false; /** request to send signal. */
let sleep_on_pause = false; /** sleep while app is not active. */
let usb_enable_dt = 200; /** Time interval for ble enabled check. */

let msgContainer = null; /** The element that displays messages on the page. */
let displayName = null; /** The element that displays the name of the contact. */
let sendBtn = null; /** The element that sends the message. */
let textInput = null; /** The element that gets the user input. */
let back = null; /** The element that redirects to index.html. */
let gps = null; /** The element that adds the GPS coordinates to the textarea. */
let maxMsg = 0; /** The maximum message ID. */
let myID = 3; /** The ID of the user. */
let showUsbStatus = null; /** The element that displays the USB status. */

document.addEventListener('deviceready', onDeviceReady, false);

/**
 * When the device is ready, open the database, set the display name, and add event listeners to the send button,
 * back button, and GPS button and display the messages.
 */
function onDeviceReady() {
  db = window.sqlitePlugin.openDatabase({
    name: 'my.db', location: 'default'
  });

  db.transaction(function (tx) {
    tx.executeSql(`CREATE TABLE IF NOT EXISTS messages ` + `(ID INTEGER, received	INTEGER, ack INTEGER, msgID	INTEGER, data TEXT)`);
    tx.executeSql(`SELECT * FROM messages`, [], function (tx, res) {
      for (let i = 0; i < res.rows.length; i++) {
        maxMsg = Math.max(res.rows.item(i).msgID, maxMsg);
        if (res.rows.item(i).ID != window.localStorage.getItem('id')) {
          continue;
        }
        displayMsg(res.rows.item(i).ID, res.rows.item(i).received, res.rows.item(i).ack, res.rows.item(i).msgID, res.rows.item(i).data);
      }
    });
  }, function (error) {
    alert('Transaction ERROR: ' + error.message);
    console.log('Transaction ERROR: ' + error.message);
  }, function () {
    console.log('Populated database OK');
  });

  displayName = document.getElementById('displayName');
  displayName.textContent = window.localStorage.getItem('name');


  textInput = document.getElementById('textInput');
  sendBtn = document.getElementById('sendBtn');
  back = document.getElementById('back');
  showUsbStatus = document.getElementById('showUsbStatus');

  /** Event listener when send button is clicked that sends the message. */
  sendBtn.addEventListener('click', sendMsg);

  /** Event listener when back is clicked that redirects to index.html. */
  back.addEventListener('click', function () {
    window.document.location = 'index.html';
  });

  /** Event listener when GPS button is clicked that adds the GPS coordinates to the textarea. */
  gps = document.getElementById('gpsBtn');
  gps.addEventListener('click', function () {
    navigator.geolocation.getCurrentPosition(function (pos) {
      textInput.value += " " + pos.coords.latitude.toString() + " " + pos.coords.longitude.toString();
    }, function (error) {
      alert('Error: ' + error.message);
    });
  });

  checkEnableUSB();
}

/** Function to open the serial connection. */
function openSerial() {
  SerialUSB.open({
    baudRate: baudrate,
    dataBits: databits,
    stopBits: stopbits,
    parity: parity,
    dtr: dtr,
    rts: rts,
    sleepOnPause: sleep_on_pause
  }, function (success_message) {
    showUsbStatus.src = 'img/usb_white_24dp.svg'
    showUsbStatus.className = 'bg-green-1000 h-full ml-auto float-right';
    SerialUSB.write(myID);
    usb_connected = true;
    SerialUSB.registerReadCallback(
      function (data) {
        readData(data);
      }, function (err) {
        console.log('Error: ' + err);
      }
    );
    SerialUSB.detached(
      function (success_message) {
        showUsbStatus.src = 'img/usb_off_white_24dp.svg'
        showUsbStatus.className = 'bg-red-500 h-full ml-auto float-right'
      }, function (err) {
        showUsbStatus.src = 'img/usb_off_white_24dp.svg'
        showUsbStatus.className = 'bg-red-500 h-full ml-auto float-right'
        usb_connected = false;
        checkEnableUSB();
      }
    );
  }, function (err) {
    console.log('Error: ' + err);
  });
}

/** Function to display a message to the page.
 * @param {string} ID - The ID of the message.
 * @param {int} received - The received status of the message.
 * @param {int} ack - The ack status of the message.
 * @param {int} msgID - The message ID.
 * @param {string} data - The message data.
 */
function displayMsg(ID, received, ack, msgID, data) {
  msgContainer = document.getElementById('massageContainer');
  var Msg = document.createElement('div');
  var newMsg = document.createElement('div');
  newMsg.innerText = data;
  newMsg.id = msgID;
  if (received == 0) {
    Msg.className = 'flex bg-black w-screen justify-end my-4';
    if (ack == 1) {
      newMsg.className = 'bg-green-1000 p-2 rounded-lg max-w-8/10 m-2';
    } else {
      newMsg.className = 'bg-red-800 p-2 rounded-lg max-w-8/10 m-2';
    }
  } else {
    Msg.className = 'flex bg-black w-screen justify-start my-4';
    newMsg.className = 'bg-purple-600 p-2 rounded-lg max-w-8/10 m-2';
  }
  Msg.appendChild(newMsg);
  msgContainer.appendChild(Msg);
  msgContainer.scrollTop = msgContainer.scrollHeight;
}

/** Function to check if USB is enabled*/
function checkEnableUSB() {
  SerialUSB.requestPermission(
    function success() {
      usb_enabled = true;
      openSerial();
    }, function error(err) {
      usb_enabled = false;
      setTimeout(checkEnableUSB, usb_enable_dt);
    });
}

/** Function that send the messages via serial and saves it to the database. */
function sendMsg() {
  if (textInput.value == '') {
    return;
  }
  maxMsg++;
  displayMsg(window.localStorage.getItem('id'), 0, 0, maxMsg, textInput.value);
  SerialUSB.write('w:' + myID + ':' + window.localStorage.getItem('id') + ':' + maxMsg + ':' + textInput.value.toString());
  //insert into database
  db.transaction(function (tx) {
    var data = textInput.value;
    tx.executeSql('INSERT INTO messages (ID, received, ack, msgID, data) VALUES (?,?,?,?,?)', [window.localStorage.getItem('id'), 0, 0, maxMsg, textInput.value]);
  }, function (error) {
    alert('Transaction ERROR: ' + error.message);
    console.log('Transaction ERROR: ' + error.message);
  }, function () {
    console.log('Populated database OK');
    textInput.value = '';
  });
}

/** Function that reads the messages from serial if the message is new
 * it gets added to the database and displayed if it's an acknowledgement it updates the database
 * and changes the color of the message.
 * @parm {Uint8Array} data The data from serial.
 * */

function readData(data) {
  var msg = String.fromCharCode(...new Uint8Array(data));

  if (msg.charAt(0) == 'a') {
    var msgData = msg.split(':');
    db.transaction(function (tx) {
      tx.executeSql('UPDATE messages SET ack = 1 WHERE msgID = ?', [msgData[1]]);
    }, function (error) {
      alert('Transaction ERROR: ' + error.message);
      console.log('Transaction ERROR: ' + error.message);
    }, function () {
      console.log('Populated database OK');
    });
    if (msgData[2].charAt(0) == window.localStorage.getItem('id').charAt(0)) { //TODO: fix this to compare numbers not strings
      document.getElementById(msgData[1]).className = 'bg-blue-500 p-2 rounded-lg max-w-8/10 m-2';
    }
  }

  if (msg.charAt(0) == 'r') {
    var msgData = msg.split(':');
    db.transaction(function (tx) {
      tx.executeSql('INSERT INTO messages (ID, received, ack, msgID, data) VALUES (?,?,?,?,?)', [msgData[1], 1, 0, msgData[2], msgData[3]]);
    }, function (error) {
      alert('Transaction ERROR: ' + error.message);
      console.log('Transaction ERROR: ' + error.message);
    }, function () {
      console.log('Populated database OK');
    });
    if (msgData[1].charAt(0) == window.localStorage.getItem('id').charAt(0)) { //TODO: fix this to compare numbers not strings{ }
      displayMsg(msgData[1], 1, 0, msgData[2], msgData[3]);
    }
    navigator.notification.beep(1);
  }
}