package _main;

import models.enums.event.EventReceiver;
import service_connection_api.ApiController;
import service_connection_api.peer_manager.PeerManager;
import service_message_history.MessageServiceController;
import service_view.UiController;
import service_coroutine.CoroutineService;
import service_event_handler.EventHandler;

public class Application {

    public static void main(String[] args) {
        // TODO: Add factory loader to eventHandler
        // sequence of instancing: 1) UI 2) setup API config 3) start API controller

        EventHandler eventHandler = EventHandler.getInstance();
        UiController uiController = UiController.getInstance();

        ApiController apiController = ApiController.getInstance();
        PeerManager peerManager = PeerManager.getInstance();
        MessageServiceController messageServiceController = MessageServiceController.getInstance();
        CoroutineService coroutine = CoroutineService.getInstance();

        eventHandler.addListener(EventReceiver.API, apiController);
        eventHandler.addListener(EventReceiver.MIDDLE_WARE, uiController);
        eventHandler.addListener(EventReceiver.COROUTINE, coroutine);
        eventHandler.addListener(EventReceiver.MESSAGE_SERVICE, messageServiceController);
        eventHandler.addListener(EventReceiver.UI, uiController);
        eventHandler.addListener(EventReceiver.PEER_MANAGER, peerManager);
        uiController.launchUi();

    }
}