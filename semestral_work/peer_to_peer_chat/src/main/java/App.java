import connection_api.ApiController;
import middle_ware.MiddleWare;
import view.Ui;

public class App {
    public static void main(String[] args) {
        Ui uiLauncher = Ui.getInstance();
        MiddleWare appController = MiddleWare.getInstance();
        ApiController apiController = ApiController.getInstance();

        // TODO: Think about connections between layers - event driven or other way



    }
}