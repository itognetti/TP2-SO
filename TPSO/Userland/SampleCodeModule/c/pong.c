// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <pong.h>

#define WIN_CONDITION 5

void drawScreen(){
    for(int i = 0; i < WIDTH; i++){
        putPixel(LINE * 4 - 3, i, GREY);
        putPixel((LINE * 4) - 4, i, GREY);
        putPixel(HEIGHT - LINE + 2, i, GREY);
        putPixel((HEIGHT - LINE) + 3, i, GREY);
    }
    for(int i = LINE * 4 - 2; i < HEIGHT - LINE + 2; i++){
        putPixel(i, WIDTH / 2, GREY);
        putPixel(i, (WIDTH / 2) + 1, GREY);
    }
}

void drawObjects(int xFrom, int xTo, int yFrom, int yTo, int color){
    for(int i = yFrom; i < yTo; i++){
        for(int j = xFrom; j < xTo; j++){
            putPixel(i, j, color);
        }
    }
}

void drawPaddle(int x, int y, int player){
    if(player == 1){
        drawObjects(x, x + PADDLE_WIDTH, y, y + PADDLE_HEIGHT, ROSE);
    }else{
        drawObjects(x, x + PADDLE_WIDTH, y, y + PADDLE_HEIGHT, SEA_GREEN);
    }

}
void undrawPaddle(int x, int y){
    drawObjects(x, x + PADDLE_WIDTH, y, y + PADDLE_HEIGHT, BLACK);
}

void drawBall(int x, int y){
    if(x >= 0 && x + BALL_RADIUS <= WIDTH){
        for(int i = y; i < y + BALL_RADIUS; i++){
            for(int j = x; j < x + BALL_RADIUS; j++){
                if(i == y && (j == x || j == x + 1 || j == x + BALL_RADIUS - 2 || j == x + BALL_RADIUS - 1)){
                    putPixel(i, j, BLACK);
                }else if(i == y + 1 && (j == x || j == x + BALL_RADIUS - 1)){
                    putPixel(i, j, BLACK);
                }else if(i == y + BALL_RADIUS - 2 && (j == x || j == x + BALL_RADIUS - 1)){
                    putPixel(i, j, BLACK);
                }else if(i == y + BALL_RADIUS - 1 && (j == x || j == x + 1 || j == x + BALL_RADIUS - 2 || j == x + BALL_RADIUS - 1)){
                    putPixel(i, j, BLACK);
                }else{
                    putPixel(i, j, WHITE);
                }
            }
        }
    }
}
void undrawBall(int x, int y){
    drawObjects(x, x + BALL_RADIUS, y, y + BALL_RADIUS, BLACK);
}

void menuPong(){
    clearScreen();
    drawScreen();
    printIn("Welcome to the first game ever PONG!", 0, (WIDTH / 2) - 18 * LETTER, WHITE);
    printIn("The first player to reach 5 points wins", LINE * 1, (WIDTH / 2) - 20 * LETTER, WHITE);
    printIn("Player 1 moves with 'W' and 'S'", LINE * 2, 0, ROSE);
    printIn("Player 2 moves with 'O' and 'L'", LINE * 2, WIDTH - 32 * LETTER, SEA_GREEN);
    printIn("Press 'SPACE' key to start", HEIGHT - 1, 0, WHITE);
    printIn("Press 'B' to return.", HEIGHT - 1, WIDTH - 20 * LETTER, WHITE);
    char c;
    while((c = getChar()) != 'b' && c != 'B'){
        if(c == ' '){
            clearScreen();
            playPong();
            break;
        }
    }
    return;
}

int p1Y, p2Y;
int ballX, ballY;
int score1 = 0, score2 = 0;
int xDirection, yDirection;
int score1Flag = 0, score2Flag = 0;     //flag de quien metio un punto
                  
void setup(){
    ballX = (WIDTH / 2) - 1; 
    ballY = HEIGHT / 2;
    p1Y = HEIGHT / 2;
    p2Y = HEIGHT / 2;

    if(score1Flag){
        xDirection = MOVEMENT;
        yDirection = MOVEMENT * getRandomSign();//-1 va a la izquierda y 1 a la derecha
        score1Flag = 0;
    }
    else if(score2Flag){
        xDirection = -MOVEMENT;
        yDirection = -MOVEMENT * getRandomSign();
        score2Flag = 0;
    }
    else {//Cuando arranca el juego
        xDirection = MOVEMENT * getRandomSign();   
        yDirection = MOVEMENT * getRandomSign();
    }
}

void playPong(){
    char points1[3] = {0};
    char points2[3] = {0};
    setup();
    printIn("Player 1 points: ", LINE * 2, 0, WHITE);
    printIn("Player 2 points: ", LINE * 2, WIDTH - 19 * LETTER, WHITE);
    printIn(itoa(score1, points1), LINE * 2, 18 * LETTER, ROSE);
    printIn(itoa(score2, points2), LINE * 2, WIDTH - 2 * LETTER, SEA_GREEN);
    printIn("Press 'B' to return.", HEIGHT - 1, WIDTH - 20 * LETTER, WHITE);
    drawScreen();
    drawPaddle(PADDLE1_X_POSITION, p1Y, 1);
    drawPaddle(PADDLE2_X_POSITION, p2Y, 2);
    drawBall(ballX, ballY);
    char c;
    while((c = getChar()) != 'b' && c != 'B' && (score1 < WIN_CONDITION && score2 < WIN_CONDITION)){
        if(score1Flag || score2Flag){
            beep(440);
            printIn(itoa(score1, points1), LINE * 2, 18 * LETTER, ROSE);
            printIn(itoa(score2, points2), LINE * 2, WIDTH - 2 * LETTER, SEA_GREEN);
            undrawPaddle(PADDLE1_X_POSITION, p1Y);
            undrawPaddle(PADDLE2_X_POSITION, p2Y);  
            setup(); //cada vez que empieza este ciclo se reinicia la posicion y se actualizan los puntajes
            drawPaddle(PADDLE1_X_POSITION, p1Y, 1);
            drawPaddle(PADDLE2_X_POSITION, p2Y, 2);
            score1Flag = 0;
            score2Flag = 0;
        }
        hold(1500000);
        moveBall();
        movePlayer(c);
    }
    if( c == 'b' || c == 'B'){
        resetGame();
        return;
    }
    clearScreen();
    if(score1 == WIN_CONDITION){
        printIn("Player 1 wins!", HEIGHT / 2, (WIDTH / 2) - 7 * LETTER, ROSE);
    }else{
        printIn("Player 2 wins!", HEIGHT / 2, (WIDTH / 2) - 7 * LETTER, SEA_GREEN);
    }
    hold(100000000);
    resetGame();
    menuPong();

}

void movePlayer(char c){
        if(c == 'w' || c == 'W' ){
            if(p1Y > TOP_BOUNDARY){
                undrawPaddle(PADDLE1_X_POSITION, p1Y);
                p1Y -= MOVEMENT;
                drawPaddle(PADDLE1_X_POSITION, p1Y, 1);
            }
        }
        if(c == 's' || c == 'S' ){
            if(p1Y + PADDLE_HEIGHT <= BOTTOM_BOUNDARY){
                undrawPaddle(PADDLE1_X_POSITION, p1Y);
                p1Y += MOVEMENT;
                drawPaddle(PADDLE1_X_POSITION, p1Y, 1);
            }
        }
        if(c == 'o' || c == 'O'){
            if(p2Y > TOP_BOUNDARY){
                undrawPaddle(PADDLE2_X_POSITION, p2Y);
                p2Y -= MOVEMENT;
                drawPaddle(PADDLE2_X_POSITION, p2Y, 2);
            }
        }
        if(c == 'l' || c == 'L'){
            if(p2Y + PADDLE_HEIGHT <= BOTTOM_BOUNDARY){
                undrawPaddle(PADDLE2_X_POSITION, p2Y);
                p2Y += MOVEMENT;
                drawPaddle(PADDLE2_X_POSITION, p2Y, 2);
            }
        }
        return;
    }

int abs(int value) {
    if (value < 0) {
        return -value;
    } else {
        return value;
    }
}

void moveBall() {
    undrawBall(ballX, ballY);

    // Verificar colisiones con los bordes superiores e inferiores
    if (ballY == TOP_BOUNDARY - 2 || ballY + BALL_RADIUS == BOTTOM_BOUNDARY) {
        yDirection = -yDirection;
    }

    // Verificar colisiones con el paddle 1 / ballX - BALL_RADIUS <= PADDLE1_X_POSITION + PADDLE_WIDTH && ballX - BALL_RADIUS >= PADDLE1_X_POSITION 
    if (ballX <= PADDLE1_X_POSITION + PADDLE_WIDTH + MOVEMENT &&
        ballY + BALL_RADIUS >= p1Y &&
        ballY - BALL_RADIUS <= p1Y + PADDLE_HEIGHT) {
        if(ballY >= p1Y + PADDLE_HEIGHT / 2 - 4 && ballY <= p1Y + PADDLE_HEIGHT / 2 + 4){
            yDirection = -yDirection;
            xDirection = MOVEMENT;
        }else if(ballY >= p1Y && ballY + BALL_RADIUS <= p1Y + PADDLE_HEIGHT){
                xDirection = -xDirection;  // Invertir la dirección horizontal
        }else if (ballY < p1Y) {
            // Pega en la esquina superior del paddle, mover hacia arriba y a la izquierda
            xDirection = -abs(xDirection);
            yDirection = -abs(yDirection);
        } else if (ballY > p1Y + PADDLE_HEIGHT) {
            // Pega en la esquina inferior del paddle, mover hacia abajo y a la izquierda
            xDirection = -abs(xDirection);
            yDirection = abs(yDirection);
        }
    }

    // Verificar colisiones con el paddle 2
    if (ballX + BALL_RADIUS >= PADDLE2_X_POSITION - 1 && ballX <= PADDLE2_X_POSITION + PADDLE_WIDTH &&
        ballY + BALL_RADIUS >= p2Y &&
        ballY - BALL_RADIUS <= p2Y + PADDLE_HEIGHT) {
        if(ballY >= p2Y + PADDLE_HEIGHT / 2 - 4 && ballY <= p2Y + PADDLE_HEIGHT / 2 + 4){
            yDirection = -yDirection;
            xDirection = -MOVEMENT;
        }else if(ballY >= p2Y && ballY + BALL_RADIUS <= p2Y + PADDLE_HEIGHT){
            xDirection = -xDirection;  // Invertir la dirección horizontal
        }else if (ballY < p2Y) {
            // Pega en la esquina superior del paddle, mover hacia arriba y a la derecha
            xDirection = abs(xDirection);
            yDirection = -abs(yDirection);
        } else if (ballY > p2Y + PADDLE_HEIGHT) {
            // Pega en la esquina inferior del paddle, mover hacia abajo y a la derecha
            xDirection = abs(xDirection);
            yDirection = abs(yDirection);
        }
    }
    ballX += xDirection;
    ballY +=yDirection;

    drawBall(ballX, ballY);

    //Regeneracion de la linea del medio
    regenerateMiddleLine();

    if(ballX+ BALL_RADIUS <= 0){
        score2Flag = 1;
        score2++;
        undrawBall(ballX, ballY);
        undrawPaddle(PADDLE1_X_POSITION, p1Y);
        undrawPaddle(PADDLE2_X_POSITION, p2Y);
    }
    if(ballX >= WIDTH){
        score1Flag = 1;
        score1++;
        undrawBall(ballX, ballY);
        undrawPaddle(PADDLE1_X_POSITION, p1Y);
        undrawPaddle(PADDLE2_X_POSITION, p2Y);
    }

}

void regenerateMiddleLine(){
    if(xDirection == MOVEMENT && ballX >= (WIDTH / 2) && ballX <= (WIDTH / 2) + BALL_RADIUS * 2){
        for (int i = BALL_RADIUS * -5; i < BALL_RADIUS * 5; i++) {
            putPixel(i + ballY, WIDTH / 2, GREY);
            putPixel(i + ballY, (WIDTH / 2) + 1, GREY);
        }
    }else if(xDirection == -MOVEMENT && ballX <= (WIDTH / 2) && ballX >= (WIDTH / 2) - BALL_RADIUS * 2){
        for (int i = BALL_RADIUS * -5; i < BALL_RADIUS * 5; i++) {
            putPixel(i + ballY, WIDTH / 2, GREY);
            putPixel(i + ballY, (WIDTH / 2) + 1, GREY);
        }
    }
}

void resetGame(){
    score1 = 0;
    score2 = 0;
    score1Flag = 0;
    score2Flag = 0;
}