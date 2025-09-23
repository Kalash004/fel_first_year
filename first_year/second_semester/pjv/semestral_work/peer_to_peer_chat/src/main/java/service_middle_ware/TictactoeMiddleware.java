package service_middle_ware;

import models.enums.tictactoe.TictactoeGameStatus;
import service_tictactoe.TictactoeGame;
import service_view.UiController;

public class TictactoeMiddleware {
    private TictactoeGame game;
    private static TictactoeMiddleware instance = null;
    public static TictactoeMiddleware getInstance() {
        if (instance == null) {
            instance = new TictactoeMiddleware();
        }
        return instance;
    }

    public TictactoeMiddleware() {
    }

    public void launchGame(boolean hasFirstTurn) {
        game = new TictactoeGame(hasFirstTurn);
        TictactoeGameStatus gameStatus = game.getGameStatus();
        UiController.getInstance().setGameStatus(gameStatus);
    }

    public void sendBoard(int x, int y) {
        game.sendBoard(x, y);
        TictactoeGameStatus gameStatus = game.getGameStatus();
        UiController.getInstance().setGameStatus(gameStatus);
    }

    private void receiveBoard(int x, int y, String sign) {
        String buttonName = "btn" + x + y;
        TictactoeGameStatus gameStatus = game.getGameStatus();
        UiController.getInstance().setGameStatus(gameStatus);
        UiController.getInstance().setReceiveBoard(buttonName, sign);
    }
}
