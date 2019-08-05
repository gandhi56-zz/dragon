//"use strict";
const WHITE = 'W'
const BLACK = 'B'
const NEUTRAL = '?'
const EMPTY = '.'
const BCOL = 1;
const WCOL = 0;
const NCOL = 2;
var SIZE = Number(stage.options.initialData2);    // default 11

var PLAYER = 0;                 // current player; white=0, black=1
var colors = [
  'white',
  'black',
  'grey'
];

var TILE_ON=-1;                  // tile that mouse is hovering over

var SIDE_LENGTH = 11;       // correction factors to adjust tile alignment
const SPACE = 24;
var moveBuffer = [];
var move = [];
var index=0;                // position in moveBuffer

var log = stage.options.initialData3;       // log file

// ~~~~~~~~~~~~~ Cell and Stone ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
function Cell(x,y,i) {
    this.key = "";
    this.value = ".";
    var id=i;
    this.img = new Polygon(x,y,SIDE_LENGTH, 6)
        .stroke('#71717f',1)
        .fill('#cecbb4')
        .attr({
            rotation:0 })
        .addTo(stage)
        .animate('.5s', {
            radius: SIDE_LENGTH,
            }, { easing: 'elasticOut'})
        .on('mouseover', function(data){
            TILE_ON=id;
            this.animate('.5s', {
                radius: 15,
            }, { easing: 'elasticOut'}); })
         .on('mouseout', function() {
             TILE_ON=-1;
             this.animate('.5s', {
                 radius: 12,
             }, { easing: 'elasticOut'}); });
};

Cell.prototype.placeStone = function(player) {
    if(player==-1) player=PLAYER;
    board.stones.addChild(new Circle(this.img.attr('x'), this.img.attr('y'), SIDE_LENGTH*0.75).fill(colors[player])
    .emit('pointerdown'));


    this.value = (player==NCOL ? '?' : colors[player][0].toUpperCase())
    board.ascii();
}

// ~~~~~~~~~~~~~~~~ Board ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

function Board() {
    this.state = new Array(SIZE);
    this.text = new Group().addTo(stage);
    this.stones = new Group();
};

Board.prototype.init = function() {

    var padding = 100;

    for (var j=0; j<SIZE; ++j) this.text.addChild(new Text(String.fromCharCode(j+49)).attr({
        x: j*SPACE + padding - 10,
        y: SPACE + 15,
        fontFamily: 'Arial, sans-serif',
        fontSize: '12',
        textFillColor: 'white',
        textStrokeColor: 'white',
        textStrokeWidth: 1
    }));
    for (var i=0; i<SIZE; ++i) {
        this.state[i] = new Array(SIZE);
        // print leftmost digits
        this.text.addChild(new Text(String.fromCharCode(i+97)).attr({
            x: -2*SPACE+padding+i*-10.25+10,
            y: i*SPACE*1.05+SPACE*2.5 + 4,
            fontFamily: 'Arial, sans-serif',
            fontSize: '12',
            textFillColor: 'white',
            textStrokeColor: 'white',
            textStrokeWidth: 1
        }));
        for (var j=0; j<SIZE; ++j)
            this.state[i][j] = new Cell(j*SPACE+padding+i*-10.25, i*SPACE+SPACE*3, SIZE*i+j);
        padding+=SPACE;

        this.text.addChild(new Text(String.fromCharCode(i+97)).attr({
            x: SIZE*SPACE+padding*0.6+SPACE*2 - 20,
            y: i*SPACE*1.05+SPACE*2.5 + 4,
            fontFamily: 'Arial, sans-serif',
            fontSize: '12',
            textFillColor: 'white',
            textStrokeColor: 'white',
            textStrokeWidth: 1
        }));
    }

    for (var j=0; j<SIZE; ++j) this.text.addChild(new Text(String.fromCharCode(j+49)).attr({
        x: j*SPACE+padding+SIZE*-10.25 - 10,
        y:SIZE*SPACE+SPACE*3.2 - 5,
        fontFamily: 'Arial, sans-serif',
        fontSize: '12',
        textFillColor: 'white',
        textStrokeColor: 'white',
        textStrokeWidth: 1
    }));

    board.text.addTo(stage);
    board.stones.addTo(stage);
};

// displays ascii representation of board through console
Board.prototype.ascii = function() {
    console.log("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ NEX ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
    var rowstr = "";
    for(var i=65;i<65+SIZE;) rowstr+="   "+ String.fromCharCode(i++);
    console.log(rowstr);

    var padding = "";
    for (var i=0; i<SIZE; ++i) {
        rowstr = padding + (i+1) + "   "
        for (var j=0; j<SIZE; ++j)
            rowstr += this.state[i][j].value + "   ";
        console.log(rowstr + " " + (i+1));
        padding += "  ";
    }

    var rowstr = new Array(SIZE*2+1).join(" ");
    for(var i=65;i<65+SIZE;) rowstr+="   "+ String.fromCharCode(i++);
    console.log(rowstr);
};

Board.prototype.onBoard = function(i,j) {
        return (i>=0  && i<SIZE && j>=0 && j<SIZE);
};

Board.prototype.get = function(i,j) {
        return this.state[i][j].value;
};

Board.prototype.set = function(i,j, player) {
        if (this.onBoard(i,j))
            this.state[j][i].placeStone(player);
};

Board.prototype.clear = function() {
    TILE_ON=-1;

    for (var i=0; i<this.state.length; ++i)
        for (var j=0; j<this.state[0].length; ++j)
            stage.removeChild(board.state[i][j].img);
    stage.removeChild(this.text);
    this.text = new Group();
    stage.removeChild(this.stones);
    this.stones = new Group();
    this.init();
};

//~~~~~~~~~~ Button~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

function Button(x,y,message, fontSize) {
    var button = new Group().addTo(stage);
    button.bg = new Rect(x, y, 150, 40, 5).attr({
    	fillGradient: gradient.radial(['#26252b', '#26252b'], 100, 50, -20),
    	strokeColor: '#CCC',
    	strokeWidth: 5
    }).addTo(button);
    button.on('mouseover', function() {
    		button.bg.animate('.2s', {
    			fillGradient: gradient.radial(['#ea8e71', '#d42f2f'], 100, 50, -20),
    			strokeWidth: 0
    		})
    	})
    	.on('mouseout', function() {
    		button.bg.animate('.2s', {
    			fillGradient: gradient.radial(['#26252b', '#26252b'], 100, 50, -20),
    			strokeWidth: 5
    		})
    	});

    button.text = new Text(message).attr({
      x: x+20,
      y: y+8,
      fontFamily: 'Didot',
      fontSize: fontSize+'px',
      textFillColor: 'white',
    }).addTo(button);

    return button;

}

var clearButton = new Button(11*SPACE*3, 2*SPACE, "Clear", 20)
    .on('click', function() {                                            // so last tile that mouse hovered over wouldn't be colored
        SIZE = stage.options.initialData2;            // update size
        console.log(SIZE);
        board.clear();
    });

var forwardButton = new Button(11*SPACE*3, 4*SPACE, String.fromCharCode(8680), 30)
    .on('click', function() {
        TILE_ON=-1;
        if(index==moveBuffer.length) {
            console.log("No more moves in the buffer");
            return;
        }
        var move = moveBuffer[index++];
        for(p of move) board.set(p.row,p.col,p.color);
    });

var reverseButton = new Button(11*SPACE*3, 6*SPACE, String.fromCharCode(8678), 30)
    .on('click', function() {
        if(index==0) {
            board.clear();
            return;
        }
        var move = moveBuffer[--index];
        for(p of move) board.set(p.row,p.col,p.color);
    });

var newGameButton = new Button(11*SPACE*3, 8*SPACE, "New Game")
    .on('click', function() {
        board.clear();
        moveBuffer = [];
    });

var board = new Board();
board.init();
board.ascii();

// ~~~~~~~~~~~~~~~~~~~~~~~~ Events ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
var banned = [];
TYPE_ONE = true;
// tile clicked event
stage.on('click', function(data) {
    if (TILE_ON!=-1) {

        var i=Math.floor(TILE_ON/SIZE);
        var j=TILE_ON%SIZE;
        var tile = board.state[Math.floor(TILE_ON/SIZE)][TILE_ON%SIZE];

        if (move.length==0 && tile.value==".") {
            move.push(colors[PLAYER][0].toUpperCase() + String.fromCharCode(j+97)+(i+1));
            tile.placeStone(PLAYER);
            TYPE_ONE = true;
        } else if (TYPE_ONE==true && tile.value=="." && move.length==1) {
            move.push("?" + String.fromCharCode(j+97)+(i+1) +";");
            tile.placeStone(NCOL);
            console.log(move);
            moveBuffer.push(move);
            move = [];
            PLAYER = 1-PLAYER;
        } else if (TYPE_ONE==false && move.length==2 && tile.value==colors[PLAYER][0].toUpperCase() && notBanned(i,j)) {
            move.push("?" + String.fromCharCode(j+97)+(i+1) +";");
            tile.placeStone(NCOL);
            console.log(move);
            moveBuffer.push(move);
            move = [];
            banned = [];
            PLAYER = 1-PLAYER;
        } else if (tile.value=="?" && move.length<=1) {
            move.push(colors[PLAYER][0].toUpperCase() + String.fromCharCode(j+97)+(i+1));
            tile.placeStone(PLAYER);
            banned.push([i,j]);
            TYPE_ONE = false;
        }


}

});

function notBanned(i,j) {
    for (t of banned) {
        if (i==t[0] && j==t[1]) return false;
    }
    return true;
}
var size=0;
stage.on('keydown', function(e) {
    if (e.keyCode==13 && size>0) {
        SIZE = size;
        size=0;
        board.clear();
        console.log(SIZE + " after change");
    } else if (!isNaN(String.fromCharCode(e.keyCode))) {
        size= size*10+Number(String.fromCharCode(e.keyCode));
        console.log(size);
    } else if (e.keyCode==85) {
        board.clear();
    }
});

var delay = 500;        // milliseconds
var player,i,j,won;
if (log!=null) {
    for (c of log) {
        switch (c) {
            case 'B':
                won=player=BCOL; break;
            case 'W':
                won=player=WCOL; break;
            case NEUTRAL:
                won=player=NCOL; break;
            case ';':
                moveBuffer.push(move);
                move = [];
                break;
            default:
                if(!isNaN(c)) {
                    i = Number(c);
                    console.log(j +" "+ i + "  made by: " + player);
                    j=j.charCodeAt(0) - 97;

                    move.push({row:--i, col:j, color: player});

                } else {
                    j=c;
                }
                won=-1;
        }
    }
    if(won!=-1) console.log(colors[won]+" has won!");
}
