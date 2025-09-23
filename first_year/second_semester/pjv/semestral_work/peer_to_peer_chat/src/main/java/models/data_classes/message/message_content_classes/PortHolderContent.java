package models.data_classes.message.message_content_classes;

import java.util.Date;
import java.util.Objects;

public class PortHolderContent {
    public int port;

    public PortHolderContent(int port) {
        this.port = port;
    }

    public PortHolderContent() {}

    @Override
    public boolean equals(Object o) {
        if (o == null || getClass() != o.getClass()) return false;
        PortHolderContent that = (PortHolderContent) o;
        return port == that.port;
    }

    @Override
    public int hashCode() {
        return Objects.hashCode(port);
    }
}
