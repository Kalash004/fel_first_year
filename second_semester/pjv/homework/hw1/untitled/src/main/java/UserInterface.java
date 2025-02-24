import java.util.ArrayList;
import java.util.Dictionary;
import java.util.Hashtable;

public class UserInterface{
    private Hashtable<String, ICommand> commands = new Hashtable<String, ICommand>();
    public UserInterface(){
        this.commands.put("add",new AdditionCommand());
    }

    
}
