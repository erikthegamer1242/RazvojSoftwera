var db = null;
var contactsList = null;
var contacts = []
var ids = []
document.addEventListener('deviceready', function() {
    db = window.sqlitePlugin.openDatabase({
        name: 'my.db',
        location: 'default',
    });
    navigator.geolocation.getCurrentPosition(onSuccess, onError);
    db.transaction(function(tx) {
        tx.executeSql('CREATE TABLE IF NOT EXISTS contacts ' +
            '(' +
            'ID     INTEGER PRIMARY KEY, ' +
            'name   TEXT NOT NULL' +
            ')'
        );
        //clear database

        // tx.executeSql('DELETE FROM contacts');
        // tx.executeSql('INSERT INTO contacts VALUES (?,?)', [1, 'EED1']);
        // tx.executeSql('INSERT INTO contacts VALUES (?,?)', [2, 'EED2']);

        tx.executeSql('SELECT * FROM contacts', [], function(tx, res) {
            for (var i = 0; i < res.rows.length; i++) {
                console.log('Row ID: ' + res.rows.item(i).ID + ' Name: ' + res.rows.item(i).name);
                // alert('Row ID: ' + res.rows.item(i).ID + ' Name: ' + res.rows.item(i).name);
                contacts.push(res.rows.item(i).name);
                ids.push(res.rows.item(i).ID);
                contactsList = document.getElementById('contactList');
                let div = document.createElement('div');
                let circle = document.createElement('span');
                let name = document.createElement('span');
                circle.innerHTML = res.rows.item(i).name[0];
                circle.classList = 'grid place-items-center mb-0.5 mt-0.5 bg-blue-500  rounded-full w-10 h-10';
                name.innerHTML = res.rows.item(i).name;
                name.classList = 'text-white text-3xl  p-2';
                div.classList = 'bg-grey-600 p-2 m-2 w-screen flex place-items-center';
                div.appendChild(circle);
                div.appendChild(name);
                div.addEventListener('click', function() {
                    alert('contact clicked');
                    window.document.location = 'messages.html';
                    window.localStorage.setItem('name', name.innerHTML);
                    window.localStorage.setItem('id', ids[contacts.indexOf(div.children[1].innerHTML)]);
                });
                contactsList.appendChild(div);
            }
        });
    }, function(error) {
        alert('Transaction ERROR: ' + error.message);
        console.log('Transaction ERROR: ' + error.message);
    }, function() {
        console.log('Populated database OK');
    });

    document.getElementById('sos').addEventListener('click', function() {
        console.log('sos clicked');
    });
    document.getElementById('addContact').addEventListener('click', function() {
        console.log('addContact clicked');
        window.document.location = 'addContact.html';
    });
});
function onSuccess(pos) {
    var lat = pos.coords.latitude;
    var lng = pos.coords.longitude;
    alert("lat : " + lat + " lng : " + lng);
}

function onError(error) {
    alert('code: ' + error.code + '\n' + 'message: ' + error.message + '\n');
}
// function contactClicked(div) {
//     alert('contact clicked');
//     window.localStorage.setItem('name', div.innerHTML);
//     var i = contacts.indexOf(div.innerHTML);
//     window.localStorage.setItem('id', res.rows.item(i).ID);
//     alert(window.localStorage.getItem('id'));
//     window.document.location = 'messages.html';
// }