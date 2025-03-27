package cz.cvut.fel.pjv.semestral.demo.model;

public class GameItem {
    protected ImageId imageId;
    protected double x;
    protected double y;

    public GameItem(ImageId imageId, double x, double y) {
        this.imageId = imageId;
        this.x = x;
        this.y = y;
    }

    public ImageId getImageId() {
        return imageId;
    }

    public double getX() {
        return x;
    }

    public double getY() {
        return y;
    }
}
