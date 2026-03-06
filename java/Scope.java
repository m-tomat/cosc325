import java.util.function.Function;

public class Scope {
    static int z=7;

    public static Function<Integer,Integer> makeAdder(int n) {
        z = 8;
        return (x) -> x+n+z;
    } 

    public static void main(String[] args) {
        Function<Integer, Integer> div5 = makeAdder(5);
        Function<Integer, Integer> div10 = makeAdder(10);
        System.out.println(div5.apply(3));
        System.out.println(div10.apply(3));
        int x = 1;
        double y = 1/3;
        System.out.println(x + " " + y);
    }
}
