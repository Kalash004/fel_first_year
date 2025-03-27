import connection_api.ApiController;
import core.Controller;
import view.Ui;

public class App {
    public static void main(String[] args) {
        Ui uiLauncher = Ui.getInstance();
        Controller appController = Controller.getInstance();
        ApiController apiController = ApiController.getInstance();

        // TODO: Think about connections between layers - event driven or other way

        appController.launch();
        apiController.launch();
        uiLauncher.launchUi();

    }
}
