import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Collections;

public class PraticalClass2_1 {
    public Map<String, List<Integer>> footWorldCup(List<Entry<Integer, String>> lf) {
        if (lf == null) {
            return null;
        }
        Map<String, List<Integer>> mapWC = new HashMap<>();
        for (Entry<Integer, String> pair : lf) {
            List<Integer> lst = mapWC.get(pair.getValue());
            if (lst == null) {
                lst = new LinkedList<>();
            }
            lst.add(pair.getKey());
            mapWC.put(pair.getValue(), lst);
        }
        for (List<Integer> lst : mapWC.values()) {
            Collections.sort(lst, Collections.reverseOrder());
        }
        return mapWC;
    }
}