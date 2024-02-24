var db = null;
var contactsList = null;
var contacts = []
var ids = []

document.addEventListener('deviceready', onDeviceReady, false);

function loadContacts(){

}

function onDeviceReady() {
	db = window.sqlitePlugin.openDatabase({
		name: 'my.db', location: 'default',
	});
	db.transaction(function (tx) {
		tx.executeSql('CREATE TABLE IF NOT EXISTS contacts ' + '(' + 'ID     INTEGER PRIMARY KEY, ' + 'name   TEXT NOT NULL' + ')');
		tx.executeSql('SELECT * FROM contacts', [], function (tx, res) {
			for (var i = 0; i < res.rows.length; i++) {
				console.log('Row ID: ' + res.rows.item(i).ID + ' Name: ' + res.rows.item(i).name);
				// alert('Row ID: ' + res.rows.item(i).ID + ' Name: ' + res.rows.item(i).name);
				contacts.push(res.rows.item(i).name);
				ids.push(res.rows.item(i).ID);
				contactsList = document.getElementById('contactList');
				let div = document.createElement('div');
				let circle = document.createElement('span');
				let name = document.createElement('span');
				// circle.innerHTML = res.rows.item(i).name[0];
				// circle.classList = 'grid place-items-center mb-0.5 mt-0.5 bg-blue-500  rounded-full w-10 h-10';
				name.innerHTML = res.rows.item(i).name;
				name.classList = 'text-white text-2xl p-2';
				div.classList = 'bg-purple-600 p-2 m-2 w-screen flex place-items-center justify-center';
				// div.appendChild(circle);
				div.appendChild(name);
				div.addEventListener('click', function () {
					// alert('contact clicked');
					window.document.location = 'messages.html';
					window.localStorage.setItem('name', name.innerHTML);
					window.localStorage.setItem('id', ids[contacts.indexOf(div.children[1].innerHTML)]);
				});
				contactsList.appendChild(div);
			}
		});
	}, function (error) {
		alert('Transaction ERROR: ' + error.message);
		console.log('Transaction ERROR: ' + error.message);
	}, function () {
		console.log('Populated database OK');
	});
	
	document.getElementById('sos').addEventListener('click', function () {
		console.log('sos clicked');
		//clear database
		db.transaction(function (tx) {
			tx.executeSql('DELETE FROM contacts');
			tx.executeSql('DELETE FROM messages');
		});
	});
	document.getElementById('addContact').addEventListener('click', function () {
		console.log('addContact clicked');
		window.document.location = 'addContact.html';
	});
}

// function contactClicked(div) {
//     alert('contact clicked');
//     window.localStorage.setItem('name', div.innerHTML);
//     var i = contacts.indexOf(div.innerHTML);
//     window.localStorage.setItem('id', res.rows.item(i).ID);
//     alert(window.localStorage.getItem('id'));
//     window.document.location = 'messages.html';
// }