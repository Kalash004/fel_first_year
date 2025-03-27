module pjv.semestral_work.kalasnikov_kolomiiets.peer_to_peer_chat {
    requires javafx.controls;
    requires javafx.fxml;

    requires org.controlsfx.controls;
    requires org.kordamp.bootstrapfx.core;

    opens pjv.semestral_work.kalasnikov_kolomiiets.peer_to_peer_chat to javafx.fxml;
    exports pjv.semestral_work.kalasnikov_kolomiiets.peer_to_peer_chat;
}