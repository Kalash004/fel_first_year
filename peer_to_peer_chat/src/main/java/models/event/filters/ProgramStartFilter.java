package models.event.filters;


import models.abstracts.event_handling.Filter;
import models.enums.event.EventReciever;
import models.enums.event.EventType;

import java.util.ArrayList;
import java.util.Arrays;

public class ProgramStartFilter extends Filter {
    public ProgramStartFilter() {
        this.recievers = new ArrayList<EventReciever>(Arrays.asList(
                EventReciever.UI,
                EventReciever.API,
                EventReciever.MIDDLE_WARE,
                EventReciever.WATCH_DOG
        ));
        this.type = EventType.PROGRAM_START;
    }
}
