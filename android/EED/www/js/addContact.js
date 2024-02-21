var db = null
var korisnik = null
var number = null
var back = null
var submit = null;

document.addEventListener('deviceready', function() {
    db = window.sqlitePlugin.openDatabase({
        name: 'my.db',
        location: 'default',
    });
    korisnik = document.getElementById('korisnik');
    number = document.getElementById('number');
    submit = document.getElementById('submit');
    back = document.getElementById('back');

    back.addEventListener('click', function () {
        // alert('back clicked')
        window.document.location = 'index.html';
    });

    submit.addEventListener('click', function() {
        // alert('submit clicked' + ' ' + number.value + ' ' + korisnik.value)
        db.transaction(function (tx) {
            tx.executeSql('INSERT INTO contacts VALUES (?,?)', [number.value, korisnik.value]);
        }, function (error) {
            alert('Transaction ERROR: ' + error.message);
            console.log('Transaction ERROR: ' + error.message);
        }, function () {
            // alert('Populated database OK' + korisnik.value + number.value)
            console.log('Populated database OK');
        });

    });
});