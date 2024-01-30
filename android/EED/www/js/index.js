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

// Event listeners
document.addEventListener('deviceready', onDeviceReady, false);

function onDeviceReady() {
	showAllowUSB();
	checkEnableUSB();
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
	showMessage('Data in: ' + String.fromCharCode(...new Uint8Array(data)));
}

function checkEnableUSB() {
	SerialUSB.requestPermission(
		function success() {
			usb_enabled = true;
			document.getElementById('allow-usb').style.display = 'none';
			document.getElementById('allow-usb-error').textContent = '';
			document.getElementById('terminal-panel').style.overflowY = 'scroll';
			updateMessagesScroll();
			openSerial();
		}, function error(err) {
			document.getElementById('allow-usb-error').textContent = 'Error: ' + err;
			usb_enabled = false;
			showAllowUSB();
			setTimeout(checkEnableUSB, usb_enable_dt);
		});
}

function showAllowUSB() {
	document.getElementById('terminal-panel').scrollTop = 0;
	document.getElementById('allow-usb').style.display = 'block';
	document.getElementById('terminal-panel').style.overflowY = 'hidden';
}

function updateMessagesScroll() {
	var bcr = document.getElementById('messages').getBoundingClientRect();
	document.getElementById('terminal-panel').scrollTop = bcr.height;
}

function showMessage(msg) {
	document.getElementById('pinkyPoruka').innerText = msg;
	var msgContainer = document.getElementById('massageContainer');
	var newMsg = document.createElement('div');
	newMsg.innerText = msg;
	newMsg.className = 'bg-pink-300 p-2 rounded-lg max-w-8/10 m-2';
	msgContainer.appendChild(newMsg);
	msgContainer.scrollTop = msgContainer.scrollHeight;
}