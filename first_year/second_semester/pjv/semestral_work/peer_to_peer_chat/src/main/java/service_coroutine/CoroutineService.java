package service_coroutine;

import models.interfaces.ICorutine;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import service_event_handler.EventHandler;
import models.events.Event;
import models.enums.event.EventType;
import models.interfaces.ICommand;
import models.interfaces.IListen;
import service_coroutine.commands.CommandStartWatchdog;

import java.util.*;
import java.util.concurrent.ConcurrentLinkedQueue;

/**
 * Watches over processes
 * if stuck terminates
 * maybe counts time ?
 */
public class CoroutineService implements Runnable, IListen {
    private static final Logger logger = LoggerFactory.getLogger(CoroutineService.class);

    private boolean runFlag = true;
    private static CoroutineService instance;
    private static final EventHandler eventHandler = EventHandler.getInstance();
    private Thread thread;
    private final ConcurrentLinkedQueue<ICorutine> coroutineQueue;
    private final HashMap<EventType, ICommand> noInputCommands = new HashMap<EventType, ICommand>(Map.of(
            EventType.PROGRAM_START, new CommandStartWatchdog(this),
            EventType.PROGRAM_END, new ICommand() {
                @Override
                public void execute(Event<?, ?> input) {
                    logger.info("Program Stopping");
                    stop();
                }
            }
    ));

    public static CoroutineService getInstance() {
        if (instance == null) {
            instance = new CoroutineService();
            instance.thread = new Thread(instance, "CoroutineService");
        }
        return instance;
    }

    private CoroutineService() {
        coroutineQueue = new ConcurrentLinkedQueue<>();
    }

    public synchronized void addCoroutine(ICorutine coroutine) {
        if (!runFlag) {
            throw new IllegalStateException("Coroutine has not been started");
        }
        logger.info("Adding coroutine: {}", coroutine);
        coroutineQueue.add(coroutine);
        logger.info("Added coroutine: {}", this.coroutineQueue.size());
    }

    @Override
    public void run() {
        logger.info("Coroutine service started");
        while (runFlag) {
            try {
                Thread.sleep(50); // A small delay to reduce CPU usage
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                break;
            }
            while (!(coroutineQueue.isEmpty())) {
                ICorutine coroutine = this.coroutineQueue.poll();
                if (coroutine == null) {
                    continue;
                }
                boolean stillRunning = coroutine.resume();
                if (stillRunning) {
                    this.coroutineQueue.add(coroutine);
                }

            }
        }
        logger.info("Coroutine service stopped {}", this.coroutineQueue.size());
    }

    @Override
    public void handleEvent(Event<?, ?> event) {
        if (!this.noInputCommands.containsKey(event.getType())) {
            // TODO: Log error
            return;
        }
        ICommand command = this.noInputCommands.get(event.getType());
        command.execute(null);
    }

    public void start() {
        runFlag = true;
        if (thread == null) {
            thread = new Thread(instance, "CoroutineService");
        }
        thread.start();
    }


    public void stop() {
        if (thread == null) return;
        logger.info("Stopping CoroutineService");
        runFlag = false;
        try {
            thread.join();
            logger.info("CoroutineService stopped !!");
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
        thread = null;
    }
}
