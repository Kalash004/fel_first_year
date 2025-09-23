package pjv;

public class RoadBike extends Bike{
    private int bike_width;

    public RoadBike(int cadence, int speed, int grear, int bw) {
        super(cadence, speed, grear);
        this.bike_width = bw;
    }
}
