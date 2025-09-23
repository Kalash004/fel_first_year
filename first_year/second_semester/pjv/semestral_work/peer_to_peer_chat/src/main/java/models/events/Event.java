package models.events;

import models.enums.event.EventReceiver;
import models.enums.event.EventSender;
import models.enums.event.EventType;

import java.util.ArrayList;

public abstract class Event<I, O> {
    protected EventType type;
    protected ArrayList<EventReceiver> receivers;
    protected EventSender sender;
    private O output;
    private I input;

    public O getOutput() {
        return output;
    }

    public void setOutput(O output) {
        this.output = output;
    }

    public I getInput() {
        return input;
    }

    public Event<I, O> setInput(I input) {
        this.input = input;
        return this;
    }

    public EventReceiver[] getReceivers() {
        return receivers.toArray(new EventReceiver[0]);
    }

    public EventSender getSender() {
        return sender;
    }

    public EventType getType() {
        return type;
    }

    @Override
    public String toString() {
        return "Event{" +
                "type=" + type +
                ", receivers=" + receivers +
                ", sender=" + sender +
                ", output=" + output +
                ", input=" + input +
                '}';
    }
}
