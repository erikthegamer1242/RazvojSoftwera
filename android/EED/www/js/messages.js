// Global variables
var device_platform;
var autoscroll = true;
var show_timestamp = true;
var usb_enabled = false;
var last_tic = 0;
var N_messages = 0;
var show_raw_input = false; // [-] Show raw input data
var N_messages_max = 100; // [-] max number of messages

var baudrate = 115200; // [bps] baud rate
var databits = 8; // [-] number of data bits
var stopbits = 1; // [-] number of stop bits
var parity = 0; // [-] type of parity
var dtr = true; // [-] data terminal ready signal
var rts = false; // [-] request to send signal
var sleep_on_pause = false; // [-] sleep while app is not active

var detect_android_notch_dt = 100; // [ms] Time interval for andorid notch check
var usb_enable_dt = 200; // [ms] Time interval for ble enabled check
var same_message_dt = 1; // [ms] Time interval allowed for same message

var send_loop; // [-] output loop identifier
var send_dt = 2000; // [ms] Time interval for output
var send_idx; // [-] output packet index

var db = null; // [-] database object

var msgContainer = null;
var bean = null;
var html = null;
var displayName = null;
var nameHeader = null;
var circle = null;
var sendBtn = null;
var textInput = null;
var back = null;
var gps = null;
var maxMsg = 0;
var myID = 2;
// Event listeners
document.addEventListener('deviceready', onDeviceReady, false);

function onDeviceReady() {
    if (onDeviceReady == true) {
        return;
    }
    alert('Device is ready');
    db = window.sqlitePlugin.openDatabase({
        name: 'my.db', location: 'default'
    });

    db.transaction(function (tx) {
        tx.executeSql('CREATE TABLE IF NOT EXISTS messages ' + '(' + 'ID     INTEGER,' + 'received	INTEGER,' + 'ack		INTEGER,' + 'msgID		INTEGER,' + 'data       TEXT' + ')');

        // clear the table
        tx.executeSql('DELETE FROM messages');
        tx.executeSql('INSERT INTO messages (ID, received, ack, msgID, data) VALUES (?,?,?,?,?)', [1, 0, 0, 1, 'Hello World']);
        tx.executeSql('INSERT INTO messages (ID, received, ack, msgID, data) VALUES (?,?,?,?,?)', [1, 1, 0, 2, 'Lorem Ipsum']);
        tx.executeSql('INSERT INTO messages (ID, received, ack, msgID, data) VALUES (?,?,?,?,?)', [1, 0, 1, 3, 'Dolor Sit']);
        tx.executeSql('INSERT INTO messages (ID, received, ack, msgID, data) VALUES (?,?,?,?,?)', [1, 1, 1, 4, 'Amet']);

        tx.executeSql('SELECT * FROM messages', [], function (tx, res) {
            for (var i = 0; i < res.rows.length; i++) {
                maxMsg = Math.max(res.rows.item(i).msgID, maxMsg);
                if (res.rows.item(i).ID != window.localStorage.getItem('id')) {
                    continue;
                }
                // alert('Row ID: ' + res.rows.item(i).ID + ' received: ' + res.rows.item(i).received + ' ack: ' + res.rows.item(i).ack + ' msgID: ' + res.rows.item(i).msgID + ' data: ' + res.rows.item(i).data);
                displayMsg(res.rows.item(i).ID, res.rows.item(i).received, res.rows.item(i).ack, res.rows.item(i).msgID, res.rows.item(i).data);
            }
        });
    }, function (error) {
        alert('Transaction ERROR: ' + error.message);
        console.log('Transaction ERROR: ' + error.message);
    }, function () {
        populatedDatabase = true;
        console.log('Populated database OK');
    });

    displayName = document.getElementById('displayName');
    displayName.textContent = window.localStorage.getItem('name');
    circle = document.getElementById('circle');
    circle.innerHTML = window.localStorage.getItem('name').charAt(0);

    textInput = document.getElementById('textInput');
    sendBtn = document.getElementById('sendBtn');
    sendBtn.addEventListener('click', sendMsg);
    back = document.getElementById('back');
    back.addEventListener('click', function () {
        // alert('back clicked')
        window.document.location = 'index.html';
    });
    gps = document.getElementById('gpsBtn');
    gps.addEventListener('click', function () {
        navigator.geolocation.getCurrentPosition(function (pos){
            textInput.value += " " + pos.coords.latitude.toString() + " " + pos.coords.longitude.toString();
        }, function (error){
            alert('Error: ' + error.message);
        });
    });

    checkHeight();
    showAllowUSB();
    checkEnableUSB();
}

function displayMsg(ID, received, ack, msgID, data) {
    msgContainer = document.getElementById('massageContainer');
    var Msg = document.createElement('div');
    var newMsg = document.createElement('div');
    newMsg.innerText = data;
    if (received == 0) {
        Msg.className = 'flex bg-gray-600 w-screen justify-end my-4';
        if (ack == 1) {
            newMsg.className = 'bg-blue-500 p-2 rounded-lg max-w-8/10 m-2';
        } else {
            newMsg.className = 'bg-red-800 p-2 rounded-lg max-w-8/10 m-2';
        }
    } else {
        Msg.className = 'flex bg-gray-600 w-screen justify-start my-4';
        newMsg.className = 'bg-pink-300 p-2 rounded-lg max-w-8/10 m-2';
    }
    Msg.appendChild(newMsg);
    msgContainer.appendChild(Msg);
    msgContainer.scrollTop = msgContainer.scrollHeight;
}


function sendMsg(){
    alert('sendMsg')
    maxMsg++;
    SerialUSB.write('w:' + myID + ':' + window.localStorage.getItem('id') + ',' + maxMsg + ':' + textInput.value);
    //insert into database
    db.transaction(function (tx) {
        tx.executeSql('INSERT INTO messages (ID, received, ack, msgID, data) VALUES (?,?,?,?,?)', [window.localStorage.getItem('id'), 0, 0, maxMsg, textInput.value]);
    }, function (error) {
        alert('Transaction ERROR: ' + error.message);
        console.log('Transaction ERROR: ' + error.message);
    }, function () {
        populatedDatabase = true;
        console.log('Populated database OK');
    });
    displayMsg(window.localStorage.getItem('id'), 0, 0, maxMsg, textInput.value);
    textInput.value = '';
}
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
        showMessage('success_message: ' + success_message);
        usb_connected = true;
        send_idx = 0;
        send_loop = setInterval(function () {
            send_idx++;
            var data_out = 'Cordova packet ' + send_idx;
            SerialUSB.write(data_out);
            showMessage('Data out: ' + data_out);
        }, send_dt);
        SerialUSB.registerReadCallback(
            function (data) {
                readData(data);
            }, function (err) {
                showMessage('Error: ' + err);
            }
        );
        SerialUSB.detached(
            function (success_message) {

            }, function (err) {
                usb_connected = false;
                showAllowUSB();
                checkEnableUSB();
                clearInterval(send_loop);
            }
        );
    }, function (err) {
        showMessage('Error: ' + err);
        clearInterval(send_loop);
    });
}

function readData(data) {
    data = String.fromCharCode(...new Uint8Array(data));
    alert('Data in: ' + data);
    // showMessage('Data in: ' + String.fromCharCode(...new Uint8Array(data)));
}

function checkEnableUSB() {
    SerialUSB.requestPermission(
        function success() {
            usb_enabled = true;
            // document.getElementById('allow-usb').style.display = 'none';
            // document.getElementById('allow-usb-error').textContent = '';
            // document.getElementById('terminal-panel').style.overflowY = 'scroll';
            // updateMessagesScroll();
            openSerial();
        }, function error(err) {
            // document.getElementById('allow-usb-error').textContent = 'Error: ' + err;
            usb_enabled = false;
            showAllowUSB();
            setTimeout(checkEnableUSB, usb_enable_dt);
        });
}

function showAllowUSB() {
    // document.getElementById('terminal-panel').scrollTop = 0;
    // document.getElementById('allow-usb').style.display = 'block';
    // document.getElementById('terminal-panel').style.overflowY = 'hidden';
}

function updateMessagesScroll() {
    // var bcr = document.getElementById('messages').getBoundingClientRect();
    // document.getElementById('terminal-panel').scrollTop = bcr.height;
}

//recursion chech hight of screen and ajust the height of msgContaner
async function checkHeight() {
    msgContainer = document.getElementById('massageContainer');
    nameHeader = document.getElementById('nameHeader');
    bean = document.getElementById('bean');
    html = document.getElementById('body');
    msgContainer.style.height = html.offsetHeight - bean.offsetHeight - nameHeader.offsetHeight + 'px';
    msgContainer.scrollTop = msgContainer.scrollHeight;
    setInterval(checkHeight, 100);
}
function readData(data) {
    var msg = String.fromCharCode(...new Uint8Array(data));
    displayMsg(window.localStorage.getItem('id'), 1, 0, 0, msg);

    if(msg.charAt(0) == 'a'){
        var msgData = msg.split(':');
        db.transaction(function (tx) {
            tx.executeSql('UPDATE messages SET ack = 1 WHERE msgID = ?', [msgData[1]]);
        }, function (error) {
            alert('Transaction ERROR: ' + error.message);
            console.log('Transaction ERROR: ' + error.message);
        }, function () {
            populatedDatabase = true;
            console.log('Populated database OK');
        });
        // displayMsg(msgData[1], 1, 0, msgData[2], msgData[3]);
    }

    if(msg.charAt(0) == 'w'){
        var msgData = msg.split(':');
        db.transaction(function (tx) {
            tx.executeSql('INSERT INTO messages (ID, received, ack, msgID, data) VALUES (?,?,?,?,?)', [msgData[1], 1, 0, msgData[2], msgData[3]]);
        }, function (error) {
            alert('Transaction ERROR: ' + error.message);
            console.log('Transaction ERROR: ' + error.message);
        }, function () {
            populatedDatabase = true;
            console.log('Populated database OK');
        });
        displayMsg(msgData[1], 1, 0, msgData[2], msgData[3]);
    }
}
