pair<int,int> histogram(vector<int> H) {
    stack<pair<int,int>> STK; // pos, height
    int mA = -1;
    pair<int, int> ANS;
    STK.push(PII(-1, 0));
    H.push_back(0);
    for (int i = 0; i < H.size(); ++i) {
        int h = H[i];
        while (STK.size() > 1 && STK.top().second >= h) {
            auto c = STK.top(); STK.pop();
            int st = STK.top().first + 1, en = i - 1;
            int area = c.second * (en - st + 1);
            if (area > mA) {
                ANS = PII(st, en);
                mA = area;
            }
        }
        STK.push(PII(i, H[i]));
    }
    return ANS;
}
