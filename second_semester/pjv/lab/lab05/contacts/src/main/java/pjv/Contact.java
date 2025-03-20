package pjv;

public class Contact
{
    private String firstName;
    private String lastName;
    private String phone;
    private int birthYear;
    private String address;

    public Contact(String firstName, String lastName, String phone, int age, String address) {
        setUpName(firstName);
        this.firstName = firstName;
        this.lastName = lastName;
        this.phone = phone;
        this.birthYear = age;
        this.address = address;
    }

    private void setUpAge(int age) {
        if (age < 1990) throw new IllegalArgumentException("Age must be at-least 1990 years");
    }

    private void setUpName(String name) {
        if (name == null || name.isEmpty()) throw new IllegalArgumentException("Name cannot be null or empty");
    }

    private void setUpLastName(String lastName) {
        if (lastName == null || lastName.isEmpty()) throw new IllegalArgumentException("Last name cannot be null or empty");
    }

    @Override
    public String toString() {
        return "Contact{" +
                "firstName='" + firstName + '\'' +
                ", lastName='" + lastName + '\'' +
                ", phone='" + phone + '\'' +
                ", age=" + birthYear +
                ", address='" + address + '\'' +
                '}';
    }
}
