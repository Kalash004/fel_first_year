package pjv;

import jdk.jshell.spi.ExecutionControl;

public class Player {
    private final String name;
    private int currentPosition;

    public Player(String name) {
        this.name = name;
    }

    public boolean advance() throws ExecutionControl.NotImplementedException {
        throw new ExecutionControl.NotImplementedException("Not implemented method");
    }
}
