package service_tictactoe;

import models.enums.tictactoe.TictactoeGameStatus;
import models.enums.tictactoe.TictactoeSpaceStatus;
import models.enums.ui_link_utils.Page;

import java.util.ArrayList;
import java.util.Arrays;

public class TictactoeGame {
    private final TictactoeSpaceStatus[][] board;
    private final TictactoeSpaceStatus mySign;
    private final TictactoeSpaceStatus oppSign;
    private TictactoeGameStatus gameStatus;
    public TictactoeGame(boolean hasFirstTurn) {
        this.board = new TictactoeSpaceStatus[3][3];
        for(int i = 0; i < 3; i++) {
            Arrays.fill(this.board[i], TictactoeSpaceStatus.EMPTY);
        }

        this.mySign = hasFirstTurn ? TictactoeSpaceStatus.X : TictactoeSpaceStatus.O;
        this.oppSign = hasFirstTurn ? TictactoeSpaceStatus.O : TictactoeSpaceStatus.X;
        this.gameStatus = hasFirstTurn ? TictactoeGameStatus.OPP_PLAYING : TictactoeGameStatus.PLAYING;
    }

    public TictactoeGameStatus getGameStatus() {
        return gameStatus;
    }

    public TictactoeSpaceStatus[][] getBoard() {
        return board;
    }

    private boolean diagonalThree(int x, int y, TictactoeSpaceStatus sign) {
        return board[(x + 1) % 3][(y + 1) % 3] == sign && board[(x + 2) % 3][(y + 2) % 3] == sign;
    }

    private boolean horizontalThree(int x, int y, TictactoeSpaceStatus sign) {
        return board[x][(y + 1) % 3] == sign && board[x][(y + 2) % 3] == sign;
    }

    private boolean verticalThree(int x, int y, TictactoeSpaceStatus sign) {
        return board[(x + 1) % 3][y] == sign && board[(x + 2) % 3][y] == sign;
    }

    private void checkWin(int x, int y) {
        if(diagonalThree(x, y, mySign) || horizontalThree(x, y, mySign) || verticalThree(x, y, mySign)) {
            this.gameStatus = TictactoeGameStatus.WIN;
        }
    }

    private void checkLoss(int x, int y) {
        if(diagonalThree(x, y, oppSign) || horizontalThree(x, y, oppSign) || verticalThree(x, y, oppSign)) {
            this.gameStatus = TictactoeGameStatus.LOSS;
        }
    }

    private void checkDraw(int x, int y) {
        boolean hasEmptySpace = false;
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                if(board[i][j] == TictactoeSpaceStatus.EMPTY) {
                    hasEmptySpace = true;
                    break;
                }
            }
        }
        if(!hasEmptySpace) {
            this.gameStatus = TictactoeGameStatus.DRAW;
        }
    }

    public void sendBoard(int x, int y) {
        board[x][y] = mySign;
        checkWin(x, y);
        if(gameStatus == TictactoeGameStatus.OPP_PLAYING) {
            checkDraw(x, y);
        }
        // send(board, status)
    }

    public void receiveBoard(int x, int y) {
        board[x][y] = oppSign;
        checkLoss(x, y);
        if(gameStatus == TictactoeGameStatus.PLAYING) {
            checkDraw(x, y);
        }
    }
}
