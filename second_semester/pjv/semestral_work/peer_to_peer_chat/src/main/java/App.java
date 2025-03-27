import connection_api.ApiController;
import core.Controller;
import view.Ui;

public class App {
    public static void main(String[] args) {
        Ui uiLauncher = new Ui();
        Controller appController = new Controller();
        ApiController apiController = new ApiController();

        // TODO: Think about connections between layers - event driven or other way

        appController.launch();
        apiController.launch();
        uiLauncher.launchUi();

    }
}
