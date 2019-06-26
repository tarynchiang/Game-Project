class Player {
    constructor(x, y) {
        this.x= x;
        this.y = y;
        this.p = document.getElementById("Runner").getContext("2d");
        // this.width = 25;
        // this.height = 80;
    }

    DrawItSelf(num) {
        // console.log('drawing', num)
        this.p.lineWidth = 3;
        switch (num) {
            case 0:
                //y=200;
                //draw the figure1
                //head
                this.p.beginPath();

                this.p.moveTo(this.x+20,this.y)
                this.p.arc(this.x, this.y, 20, 0, Math.PI * 2);
                this.p.moveTo(this.x, this.y-20);
                this.p.lineTo(this.x, this.y-30);
                this.p.moveTo(this.x - 10, this.y-17.3);
                this.p.lineTo(this.x - 15, this.y-25);
                this.p.moveTo(this.x + 10, this.y-17.3);
                this.p.lineTo(this.x + 15, this.y-25);
                //body
                this.p.moveTo(this.x, this.y+20);
                this.p.lineTo(this.x, this.y+60);
                //legs
                this.p.lineTo(this.x + 20, this.y+90);
                this.p.moveTo(this.x, this.y+60);
                this.p.lineTo(this.x - 20, this.y+90);
                //arms
                this.p.moveTo(this.x, this.y+35);
                this.p.lineTo(this.x + 20, this.y+45);
                this.p.moveTo(this.x, this.y+35);
                this.p.lineTo(this.x - 20, this.y+45);
                break;
            case 1:
                //behavior 1 
                //head
                this.p.beginPath();

                this.p.moveTo(this.x, this.y);
                this.p.arc(this.x - 20, this.y, 20, 0, Math.PI * 2);
                this.p.moveTo(this.x - 20, this.y - 20);
                this.p.lineTo(this.x - 20, this.y - 30);
                this.p.moveTo(this.x - 30, this.y - 17.3);
                this.p.lineTo(this.x - 35, this.y - 25);
                this.p.moveTo(this.x - 10, this.y - 17.3);
                this.p.lineTo(this.x - 5, this.y - 25);
                //body
                this.p.moveTo(this.x - 25, this.y + 20);
                this.p.lineTo(this.x - 40, this.y + 50);
                //legs 
                this.p.lineTo(this.x - 25, this.y + 65);
                this.p.lineTo(this.x - 20, this.y + 90);
                this.p.moveTo(this.x - 40, this.y + 50);
                this.p.lineTo(this.x - 60, this.y + 70);
                this.p.lineTo(this.x - 80, this.y + 50);
                //arms
                this.p.moveTo(this.x - 33, this.y + 35);
                this.p.lineTo(this.x - 15, this.y + 45);
                this.p.lineTo(this.x + 10, this.y + 35);
                this.p.moveTo(this.x - 33, this.y + 35);
                this.p.lineTo(this.x - 50, this.y + 25);
                this.p.lineTo(this.x - 70, this.y + 35);
                break;
            case 2:
                //this.x+50
                //behavior 2
                //head
                this.p.beginPath();
                this.p.moveTo(this.x, this.y + 5);
                this.p.arc(this.x - 20, this.y + 5, 20, 0, Math.PI * 2);
                this.p.moveTo(this.x - 20, this.y - 15);
                this.p.lineTo(this.x - 20, this.y - 25);
                this.p.moveTo(this.x - 30, this.y - 12.3);
                this.p.lineTo(this.x - 35, this.y - 20);
                this.p.moveTo(this.x - 10, this.y - 12.3);
                this.p.lineTo(this.x - 5, this.y - 20);

                //body
                this.p.moveTo(this.x - 25, this.y + 25);
                this.p.lineTo(this.x - 35, this.y + 50);

                //legs
                this.p.lineTo(this.x - 20, this.y + 70);
                this.p.lineTo(this.x - 20, this.y + 90);
                this.p.moveTo(this.x - 20, this.y + 70);
                this.p.lineTo(this.x - 35, this.y + 80);

                //arms
                this.p.moveTo(this.x - 30, this.y + 35);
                this.p.lineTo(this.x - 20, this.y + 50);
                this.p.lineTo(this.x, this.y + 40);
                this.p.moveTo(this.x - 30, this.y + 35);
                this.p.lineTo(this.x - 50, this.y + 33);
                this.p.lineTo(this.x - 60, this.y + 50);
                break;
            case 3:
                //this.x+100 
                //behavior 3
                //head
                this.p.beginPath();
                this.p.moveTo(this.x, this.y);
                this.p.arc(this.x - 20, this.y, 20, 0, Math.PI * 2);
                this.p.moveTo(this.x - 20, this.y - 20);
                this.p.lineTo(this.x - 20, this.y - 30);
                this.p.moveTo(this.x - 30, this.y - 17.3);
                this.p.lineTo(this.x - 35, this.y - 25);
                this.p.moveTo(this.x - 10, this.y - 17.3);
                this.p.lineTo(this.x - 5, this.y - 25);
                //body
                this.p.moveTo(this.x - 20, this.y + 20);
                this.p.lineTo(this.x - 25, this.y + 55);
                //legs
                this.p.lineTo(this.x - 50, this.y + 90);
                this.p.moveTo(this.x - 25, this.y + 55);
                this.p.lineTo(this.x - 5, this.y + 55);
                this.p.lineTo(this.x - 10, this.y + 80);

                //arms
                this.p.moveTo(this.x - 20, this.y + 20);
                this.p.lineTo(this.x - 40, this.y + 35);
                this.p.lineTo(this.x - 25, this.y + 50);
                this.p.moveTo(this.x - 23, this.y + 40);
                this.p.lineTo(this.x, this.y + 40);
                break;
            case 4:
                //this.x+150
                //behavior 4
                //head
                this.p.beginPath();
                this.p.moveTo(this.x, this.y - 10);
                this.p.arc(this.x - 20, this.y - 10, 20, 0, Math.PI * 2);
                this.p.moveTo(this.x - 20, this.y - 30);
                this.p.lineTo(this.x - 20, this.y - 40);
                this.p.moveTo(this.x - 30, this.y - 27.3);
                this.p.lineTo(this.x - 35, this.y - 35);
                this.p.moveTo(this.x - 10, this.y - 27.3);
                this.p.lineTo(this.x - 5, this.y - 35);
                //body
                this.p.moveTo(this.x - 20, this.y + 10);
                this.p.lineTo(this.x - 25, this.y + 35);

                //legs
                this.p.lineTo(this.x - 5, this.y + 33);
                this.p.lineTo(this.x - 10, this.y + 53);
                this.p.moveTo(this.x - 25, this.y + 35);
                this.p.lineTo(this.x - 25, this.y + 60);
                this.p.lineTo(this.x - 45, this.y + 80);

                //arms
                this.p.moveTo(this.x - 20, this.y + 10);
                this.p.lineTo(this.x - 5, this.y + 25);
                this.p.lineTo(this.x + 10, this.y + 15);
                this.p.moveTo(this.x - 20, this.y + 10);
                this.p.lineTo(this.x - 40, this.y + 18);
                this.p.lineTo(this.x - 25, this.y + 30);
                break;
            case 5:
                //this.x+200
                //behavior 5
                //head 
                this.p.beginPath();
                this.p.moveTo(this.x, this.y - 5);
                this.p.arc(this.x - 20, this.y - 5, 20, 0, Math.PI * 2);
                this.p.moveTo(this.x - 20, this.y - 25);
                this.p.lineTo(this.x - 20, this.y - 35);
                this.p.moveTo(this.x - 30, this.y - 22.3);
                this.p.lineTo(this.x - 35, this.y - 30);
                this.p.moveTo(this.x - 10, this.y - 22.3);
                this.p.lineTo(this.x - 5, this.y - 30);

                //body
                this.p.moveTo(this.x - 22, this.y + 14);
                this.p.lineTo(this.x - 30, this.y + 45);

                //legs
                this.p.lineTo(this.x - 10, this.y + 48);
                this.p.lineTo(this.x + 10, this.y + 63);
                this.p.moveTo(this.x - 30, this.y + 45);
                this.p.lineTo(this.x - 55, this.y + 55);
                this.p.lineTo(this.x - 75, this.y + 45);

                //arms
                this.p.moveTo(this.x - 25, this.y + 28);
                this.p.lineTo(this.x - 10, this.y + 33);
                this.p.lineTo(this.x + 10, this.y + 23);
                this.p.moveTo(this.x - 25, this.y + 23);
                this.p.lineTo(this.x - 45, this.y + 15);
                this.p.lineTo(this.x - 65, this.y + 25);
                break;
            case 6:
                //behavior 6 with obstacle jump
                this.p.beginPath();
                this.p.moveTo(this.x, this.y-50 - 5);
                this.p.arc(this.x - 20, this.y-50 - 5, 20, 0, Math.PI * 2);
                this.p.moveTo(this.x - 20, this.y-50 - 25);
                this.p.lineTo(this.x - 20, this.y-50 - 35);
                this.p.moveTo(this.x - 30, this.y-50 - 22.3);
                this.p.lineTo(this.x - 35, this.y-50 - 30);
                this.p.moveTo(this.x - 10, this.y-50 - 22.3);
                this.p.lineTo(this.x - 5, this.y-50 - 30);

                //body
                this.p.moveTo(this.x - 22, this.y-50 + 14);
                this.p.lineTo(this.x - 30, this.y-50 + 45);

                //legs
                this.p.lineTo(this.x - 10, this.y-50 + 48);
                this.p.lineTo(this.x + 10, this.y-50 + 63);
                this.p.moveTo(this.x - 30, this.y-50 + 45);
                this.p.lineTo(this.x - 55, this.y-50 + 55);
                this.p.lineTo(this.x - 75, this.y-50 + 45);

                //arms
                this.p.moveTo(this.x - 25, this.y-50 + 28);
                this.p.lineTo(this.x - 10, this.y-50 + 33);
                this.p.lineTo(this.x + 10, this.y-50 + 23);
                this.p.moveTo(this.x - 25, this.y-50 + 23);
                this.p.lineTo(this.x - 45, this.y-50 + 15);
                this.p.lineTo(this.x - 65, this.y-50 + 25);
                break;
        }
        this.p.stroke();
    }
    
    moveLeft(){
        this.x -=5;
    }

    checkbound(){
        if(this.x > 1100){
            this.x = 1100;
        }
        if(this.x < 100){
            this.x =100;
        }
    }
    auto(){

    }
}