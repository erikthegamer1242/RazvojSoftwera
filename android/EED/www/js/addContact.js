let /** @type{?globalDatabaseObject} */ db = null; /** Object used to access a sqlite database. */

document.addEventListener('deviceready', onDeviceReady); /** Event listener when app is started. */

function onDeviceReady() {
    db = window.sqlitePlugin.openDatabase({
        name: 'my.db',
        location: 'default',
    });
    back = document.getElementById('back');

    /** Event listener when back is clicked that redirects to index.html. */
    back.addEventListener('click', function () {
        window.document.location = 'index.html';
    });

    let submit = document.getElementById('submit');

    /** Event listener when submit is clicked that adds a new contact to the database. */
    submit.addEventListener('click', function() {
        let name = document.getElementById('korisnik');
        let id = document.getElementById('number');
        db.transaction(function (tx) {
            tx.executeSql('INSERT INTO contacts VALUES (?,?)', [id.value, name.value]);
        }, function (error) {
            alert('Transaction ERROR: ' + error.message);
            console.log('Transaction ERROR: ' + error.message);
        }, function () {
            console.log('Populated database OK');
        });

    });
};