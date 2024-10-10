import java.util.LinkedList;

public class PracticalClass2_2 {
    public LinkedList<Integer> CalcMMS(LinkedList<Integer> serie, Integer period) {
        if (serie == null) {
            return null;
        }
        if (period == 0 || period > serie.size()) {
            return null;
        }
        LinkedList<Integer> mms = new LinkedList<>();
        LinkedList<Integer> buffer = new LinkedList<>();
        Integer sum = 0;
        for (Integer num : serie) {
            buffer.add(num);
            sum += num;
            if (buffer.size() < period) {
                mms.add(0);
            } else if (buffer.size() == period) {
                mms.add(sum / period);
                num = buffer.removeFirst();
                sum -= num;
            }
        }
        return mms;
    }
}