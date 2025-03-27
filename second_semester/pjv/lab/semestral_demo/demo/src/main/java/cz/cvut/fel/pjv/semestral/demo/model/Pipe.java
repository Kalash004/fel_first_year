package cz.cvut.fel.pjv.semestral.demo.model;

public class Pipe extends GameItem{
    private final double speedX;
    private final double bgrWidth;

    public Pipe(ImageId imageId, double x, double y, double speedX, double bgrWidth) {
        super(imageId, x, y);
        this.speedX = speedX;
        this.bgrWidth = bgrWidth;
    }

    public void update() {
        x += speedX;
        if (x + imageId.getWidth() < 0){
            x = bgrWidth;
        }
    }
}
