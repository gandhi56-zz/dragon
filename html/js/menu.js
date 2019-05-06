
function ShowHelp() {
    alert("Nex is a variant of the two player strategy board game Hex."
        +" Players alternate making one of the following moves: place a stone of their color and a neutral stone on distinct empty tiles, or convert two distinct neutral stones to their color and then turn one of their other stones neutral."
        + "Each player have the same objective of connecting their opposite sides together by means of a chain their stones."
        + "\n\nPvP:\n"
            +"Upon loading, you may proceed to play a game with another player by alternating moves. By clicking on an empty tile, you commit to making the \"generate\" move and a stone of your color appears where you just clicked."
            +"In order to finish your move, you must select another empty tile to place a neutral stone. Instead, if you choose the \"convert\" move by clicking on one of your existing stones, it will turn neutral. Then, you must click on two other neutral stones to finish your move."
        + "\n\nTo play a log file:\n"
        + "Either copy and paste the log file into the textarea or upload the file directly, then press the Upload button. You can play and rewind through the game by using the buttons with arrows."
        + "\n\nSize:\n"
        + "To change the size, enter the new size in the textbox and press the Resize button.");
}

function clear() {}

function exit() {}

// ~~~~~~~~~~~~~~~~~~~~~~ File Read/Write ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

function openInput(e) {
    var file = e.target.files[0];
    if (!file) {
        alert("File not found!!");
		return;
    }
    var reader = new FileReader();
    reader.onload = function(e) {
        var contents = e.target.result;
        displayContents(contents);
        saveLog(contents, 'log');
    };
    reader.readAsText(file);
}

function displayContents(contents) {
  var element = document.getElementById('file-output');
  element.value = contents;
}

function save(){
    var text = document.getElementById("file-output").value;
    var blob = new Blob([text], { type: "text/plain"});
    var anchor = document.createElement("a");
    anchor.download = "my-filename.txt";
    anchor.href = window.URL.createObjectURL(blob);
    anchor.target ="_blank";
    anchor.style.display = "none"; // just to be safe!
    document.body.appendChild(anchor);
    anchor.click();
    document.body.removeChild(anchor);
 }


window.onbeforeunload = function() {
    try {
        localStorage.setItem("logFile", document.getElementById('file-output').value);
        localStorage.setItem("sizeFile", document.getElementById('field_size').value);
    } catch(e) {}
}

window.onload = function() {
    try {
        var log = localStorage.getItem("logFile");
        var size = localStorage.getItem("sizeFile");
        if (log !== null) document.getElementById('file-output').value=log;
        if (size !== null) document.getElementById('field_size').value=size;
    } catch(e) {}
}
function Reload() {
    window.location.reload();
}
