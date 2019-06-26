let game = document.getElementById('Runner').getContext('2d');

let player1 = new Player(50, 200);
let player2 = new Player(50, 470);
let Complayer = new Player(50, 470);
let i = 1;
let j = 1;
let allTheObstacles1 = [];
let allTheObstacles2 = [];
let allCloud1 = [];
let allCloud2 = [];
let counter = 0;
let bumpsign = new BumpSign();
let result;
let finish;


//excecute whole Game
$(document).ready(function () {
    $("#restart-button").toggle();
    $("#game-page").toggle();

    $("#start").click(function(){
        $("#info").toggle();
        $("#game-page").toggle();
        StartGame();
    });
});


//draw everything on the canvas
function drawEverything() {
    drawthetrack();
    player1.DrawItSelf(i);
    player2.DrawItSelf(j);
    allTheObstacles1.forEach((eachObstacle) => {
        eachObstacle.drawItSelf();
        if (eachObstacle.x < 20) {
            allTheObstacles1.shift();
        }
    })
    allTheObstacles2.forEach((eachObstacle) => {
        eachObstacle.drawItSelf();
        if (eachObstacle.x < 20) {
            allTheObstacles2.shift();
        }
    })
    allCloud1.forEach((cloud) => {
        cloud.drawItSelf();
        if (cloud.x < 20) {
            allCloud1.shift();
        }
    })
    allCloud2.forEach((cloud) => {
        cloud.drawItSelf();
        if (cloud.x < 20) {
            allCloud2.shift();
        }
    })
}
//move everything to the left
function moveLeftAll() {
    allTheObstacles1.forEach((eachObstacle) => {
        eachObstacle.moveLeft();
    })
    allTheObstacles2.forEach((eachObstacle) => {
        eachObstacle.moveLeft();
    })
    allCloud1.forEach((cloud) => {
        cloud.moveLeft();
    })
    allCloud2.forEach((cloud) => {
        cloud.moveLeft();
    })

    player1.moveLeft();
    player2.moveLeft();
}

function checkboundAll() {
    player1.checkbound();
    player2.checkbound();
}

//control players
$(document).keydown(function (e) {
    console.log('hi');

    if (e.key === 'k' || e.key === 'l') {
        i++;
        player1.x += 20;
    }
    if (e.key === 'a' || e.key === 's') {
        j++;
        player2.x += 20;
    }
    if (i > 5) {
        i = 1;
    }
    if (j > 5) {
        j = 1;
    }
    if (e.key === 'j') {
        player1.DrawItSelf(4);
        player1.x += 90;
        setTimeout(player1.DrawItSelf(6), 100);
        player1.x += 90;
        setTimeout(player1.DrawItSelf(2), 100);
        i = 1;
    }
    if (e.key === 'd') {
        player2.DrawItSelf(4);
        player2.x += 90;
        setTimeout(player2.DrawItSelf(6), 100);
        player2.x += 90;
        setTimeout(player2.DrawItSelf(2), 100);
        j = 1;
    }
});


//detect the collision
function detectCollisions() {

    allTheObstacles1.forEach((obs) => {

        if (player1.x < obs.x + obs.width && player1.x > obs.x) {
            bumpsign.drawItSelf(player1.x, player1.y);
            setTimeout(() => {
                game.clearRect(0, 0, 1200, 600);
                clearInterval(result);
                game.font = '100px serif';
                game.strokeText('Player 2 Wins.', 300, 300);
                $("#restart-button").toggle();
            }, 200)
        }
    })
    allTheObstacles2.forEach((obs) => {
        if (player2.x < obs.x + obs.width && player2.x > obs.x) {
            bumpsign.drawItSelf(player2.x, player2.y);
            setTimeout(() => {
                game.clearRect(0, 0, 1200, 600);
                clearInterval(result);
                game.font = '100px serif';
                game.strokeText('Player 1 Wins.', 300, 300);
                $("#restart-button").toggle();
            }, 200)
        }
    })
}

//check who wins 
function checkWin() {
    if (finish) {
        if (player1.x >= finish.x) {
            game.clearRect(0, 0, 1200, 600);
            clearInterval(result);
            game.font = '100px serif';
            game.strokeText('Player 1 Wins.', 300, 300);
            $("#restart-button").toggle();
        }
        if (player2.x >= finish.x) {
            game.clearRect(0, 0, 1200, 600);
            clearInterval(result);
            game.font = '100px serif';
            game.strokeText('Player 2 Wins.', 300, 300);
            $("#restart-button").toggle();
        }
    }
}

//restart the game 
$("#restart-button").click(function(){
    location.reload();
});

//Game function
function StartGame(){
    result = setInterval(() => {
        game.clearRect(0, 0, 1200, 600);
        moveLeftAll();
        checkboundAll();
        if (counter % 100 === 7) {
            let random = Math.floor(Math.random() * 10);
            let i = Math.floor(Math.random()*3);
            if (random > 2) {
                let obs = new Obstacle(1100, 250,i);
                allTheObstacles1.push(obs);

            }
        }
        if (counter % 100 === 7) {
            let random = Math.floor(Math.random() * 11);
            let i = Math.floor(Math.random()*3);
            if (random > 2) {
                let obs = new Obstacle(1100, 520,i);
                allTheObstacles2.push(obs);

            }
        }
        if (counter % 50 === 0) {
            let random1 = Math.floor(Math.random() * 50 + 30);
            if (random1 > 2) {
                let cloud1 = new Cloud(1100, random1);
                allCloud1.push(cloud1);
            }
            let random2 = Math.floor(Math.random() * 50 + 310);
            if (random2 > 4) {
                let cloud2 = new Cloud(1100, random2);
                allCloud2.push(cloud2);
            }
        }
        if (counter > 500) {
            finish = new FinishLine();
            finish.drawItSelf();
        }
        drawEverything();
        detectCollisions();
        checkWin();
        counter++;

    }, 50);
}