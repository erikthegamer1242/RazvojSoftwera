let /** @type{?globalDatabaseObject} */ db = null;/** Object used to access a sqlite database. */
var debuging = true;  //TODO set to false before release

document.addEventListener('deviceready', onDeviceReady); /** Event listener when app is started. */

/**
 * Adds a contact to the page.
 * @param item Equal to row of the contacts table.
 */
function addContactToPage(item){
//	if (debuging) alert(`addContactToPage(${item.name},${item.ID})`);
	let div = document.createElement('div');
	div.classList = 'bg-purple-600 p-2 m-2 w-screen flex place-items-center justify-center';
	let name = document.createElement('span');
	name.innerHTML = item.name;
	name.classList = 'text-white text-2xl p-2';
	div.appendChild(name);
	div.addEventListener('click', contactClicked, false);
	div.name = item.name;
	div.ID = item.ID;
	let contactsList = document.getElementById('contactList');
	contactsList.appendChild(div);
}

/**
 * Load contacts from database and displays them to the page.
 */
function loadContacts(){
	// if (debuging) alert(`loadContacts()`);
	db.transaction(function (tx) {
		tx.executeSql(`CREATE TABLE IF NOT EXISTS contacts ` + `(` + `ID     INTEGER PRIMARY KEY, ` + `name   TEXT NOT NULL` + `)`);
		tx.executeSql(`SELECT * FROM contacts`, [], function (tx, res) {
			for (let i = 0; i < res.rows.length; i++) {
				console.log('Row ID: ' + res.rows.item(i).ID + ' Name: ' + res.rows.item(i).name);
				addContactToPage(res.rows.item(i));
			}
		});
	}, function (error) {
		// if (debuging) alert('Transaction ERROR: ' + error.message);
		console.log('Transaction ERROR: ' + error.message);
	}, function () {
		console.log('Populated database OK');
	});
}

/**
 * When a contact is clicked, save the name and id to local storage and redirect to messages.html.
 * @param event Default event object from eventListener.
 */
function contactClicked(event) {
	// if (debuging) alert(`contactClicked(${event.currentTarget.name},${event.currentTarget.ID})`);
	window.localStorage.setItem('name', event.currentTarget.name);
	window.localStorage.setItem('id',  event.currentTarget.ID);
	window.document.location = 'messages.html';
}

/**
 * When the device is ready, open the database, load contacts, and add event listeners to
 * SOS button and Add Contact button.
 */
//TODO add sos button
function onDeviceReady() {
	// navigator.splashscreen.hide();
	// if (debuging) alert(`onDeviceReady()`);
	db = window.sqlitePlugin.openDatabase({
		name: 'my.db', location: 'default',
	});
	loadContacts();

	document.getElementById('sos').addEventListener('click', function () {
		console.log('sos clicked');
		//clear database
		db.transaction(function (tx) {
			tx.executeSql('DELETE FROM contacts');
			tx.executeSql('DELETE FROM messages');
		});
	});
	document.getElementById('addContact').addEventListener('click', function () {
		window.document.location = 'add_contact.html';
	});
}