package pjv;

public class MountainBike extends Bike {
    private String suspension;

    public MountainBike(int cadence, int speed, int grear, String suspension) {
        super(cadence, speed, grear);
        this.suspension = suspension;
    }

}
