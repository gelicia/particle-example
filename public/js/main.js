var rootURL = "http://localhost:3000";

function init() {
    $.get({url: rootURL + "/temperature"}).then( function(temperature) {
        $('#temperature')[0].innerText = parseInt(temperature) + "°F";
    });

    $('button').click(function(evtData){
        //get whatever comes after the underscore for the index
        var idx =  evtData.target.id.substring(evtData.target.id.indexOf('_')+1);
        var red = $('#red').val();
        var grn = $('#green').val();
        var blu = $('#blue').val();

        $.post({
            url: rootURL + "/setLED",
            data: {
                ledIdx: idx,
                ledR: red,
                ledG: grn,
                ledB: blu
            }
        });
    });
}