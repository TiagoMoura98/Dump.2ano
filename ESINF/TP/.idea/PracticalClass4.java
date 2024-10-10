public Map<String, Map<String, Integer>> numberMsgsSent() {
    Map<String, Map<String, Integer>> result = new HashMap<>();
    for (Message msg.this.getCaixaOut()){
        if (result.get(msg.getSender()) == null){
            result.put(msg.getSender(), new HashMap<>());
            Map<String, Integer> numMsgs = result.get(msg.getSender());
            for (String receiver : msg.getReceiver()){
                Integer num = numMsgs.get(receiver);
                if (num == null){
                    num =  0;
                } else {
                    numMsgs.put(receiver, num + 1);
                }
            }

        }
    }
    return result;
}