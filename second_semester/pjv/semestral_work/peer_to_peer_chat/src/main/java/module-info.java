module pjv.semestral_work.kalasnikov_kolomiiets.peer_to_peer_chat {
    requires javafx.controls;
    requires javafx.fxml;

    requires org.controlsfx.controls;
    requires org.kordamp.bootstrapfx.core;
    requires java.logging;

    opens view to javafx.fxml;
    exports view;
}