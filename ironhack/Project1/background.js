//draw the track background
function drawthetrack(){
    let b = document.getElementById('Runner').getContext('2d');
    //draw the first line 
    b.moveTo(0,300);
    b.lineTo(1175,300);
    //draw the second line
    b.moveTo(0,570);
    b.lineTo(1175,570);
    
    b.stroke();
}

class Cloud{
    constructor(x,y){
        this.x = x;
        this.y = y;
        this.cl = document.getElementById("Runner").getContext("2d");
        this.img = new Image();
        this.img.src = 'image/cloud.png';
    }
    
    drawItSelf(){
        this.cl.drawImage(this.img,this.x,this.y,100,50);
    }
    moveLeft(){
        this.x -= 5;
    }
}

class BumpSign{
    constructor(){
        this.bs = document.getElementById("Runner").getContext("2d");
        this.img = new Image();
        this.img.src = 'image/bumpsign.png';
    }
    drawItSelf(x,y){
        console.log('bump');
        this.bs.drawImage(this.img,x,y,100,50);

    }
}

class FinishLine{
    constructor(){
        this.x = 1100;
        this.fl = document.getElementById("Runner").getContext("2d");
        this.img = new Image();
        this.img.src = 'image/finishline1.png';
    }
    drawItSelf(){
        this.fl.drawImage(this.img,1100,50,70,500);
    }
}

