package connection_api;

public class ApiController {
    private static ApiController instance = new ApiController();

    private ApiController() {
    }

    public static ApiController getInstance() {
        return instance;
    }

    public void launch() {
        //TODO:
    }
}
