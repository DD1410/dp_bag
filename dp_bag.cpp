#include<iostream>
#include<vector>

using namespace std;

//总结一下dp动态规划里常见的各种背包问题，当笔记，供学习用

/*
*第一种，最简单的01背包
*背包限重target，第i件物品价值value[i]，第i件物品重量weight[i]，每件物品只有一件，能装的最大价值
*先写一下暴力递归，这个方法从B站左神--左程云那里学的
*@func : 到第i个物品为止（第i个物品还没操作），背包容量还剩rest，针对第i个物品进行操作，能得到的最大价值
*/
int dp_01bag(int rest, int i, vector<int>& value, vector<int>& weight){
    if (rest < 0 || i >= value.size())
    return 0;
    
    int p1 = 0, p2 = 0;
    p1 = dp_01bag(rest, i - 1, value, weight);//第i件物品没选，那么这种情况下的最大价值与对第i-1个物品操作的时候一样，对应01背包的0
    p2 = dp_01bag(rest - weight[i], i - 1, value, weight) + value[i];//第i件选了，这种情况的最大价值与对第i-1个物品操作的时候，差别在于rest需要减去物品i的重量，加上物品i的价值，对应01背包的1

    return max(p1, p2);

}

/*下面翻译成递推
*dp_01bag函数变成dp[i][rest],他有三个信息：i 表示考虑到i为止；
*rest表示当前背包剩余的重量
*dp[i][rest]这个位置存的值，表示还剩rest的重量，对第i件物品进行操作，可以得到的最大价值
*下面要做的就是填dp[][]的值
*用一个函数来填，假设有n件物品，dp的定义为 <vector<vector<int>> dp(n, vector<int>(target + 1, 0))
*记住这里的dp[i][rest]的定义
*/
void fill_dp(int target, vector<int>& value, vector<int>& weight, vector<vector<int>>& dp){
    int n = value.size();
    for (int j = target; j >= weight[0]; j--) {//记住这里的dp[i][rest]的定义, i 表示考虑到i为止,当dp[i][rest]的i=0时，表示考虑到0为止，也就是后面都不考虑了，能有的最大价值，所以肯定只要rest>weight[0]就把第0件装进去，这时是价值最大的
        dp[0][j] = value[0];
    }

    for (int i = 1; i < n; i++){
        for (int rest = 0; rest <= target; rest++){ //如果rest=0了，那不肯定装不下了，不是dp[i][0]=0吗？不是的，万一weight[i]=0呢，还能装
            if (rest >= weight[i])
            dp[i][rest] = dp[i - 1][rest - weight[i]] + value[i];

            dp[i][rest] = max(dp[i][rest], dp[i - 1][rest]); //这三行就是dp[i][rest] = max(dp[i - 1][rest - weight[i]] + value[i], dp[i - 1][rest]);
        }
    }
}
/*
*注意到dp的状态转移方程都与i-1有关，下面做一个滚动数组的空间优化，二维dp变一维
*也就是说，dp的每一层都与他的上面一层有关，并且与前面的列（rest-weight[i]）有关，所以整体是一个左上向右下推的过程
*定义dp为 vector<int> dp(target + 1)
*/
void fill_dp(int target, vector<int>& value, vector<int>& weight, vector<int>& dp){
    int n = value.size();
    for (int i = 0; i < n; i++){//我们一行一行往下卷
        for (int j = target; j >= weight[i]; j--){//一次算一行并往下卷一行
            dp[j] = max(dp[j], dp[j - weight[i]] + value[i]);//当上一行卷下来，还没有进行max比较的时候，dp[j]不就是上一行的值吗？
            //那为什么j要从右边开始，也就是为什么j一定要--？因为我们当前值的计算用到了"上边卷下来的左边的值"，如果我们从左边开始，那么从上一行卷下来的左边的值就被破坏了
            //就不等价于dp[i-1][restrest - weight[i]]了，而是dp[i][restrest - weight[i]]了，因为我们从左边开始的话左边的值被更新了
            //我们当前的值运算，用不到右边的值，所以要从右边开始，确保上边卷下来的值，要被使用的时候，没有被破坏
        }
    }
}


/*
*与01背包类似的，还有完全背包，就是每件物品有无数个
*在01背包里，考虑过i就不能再考虑了，必须去考虑i-1或更前面的物品所以是dp[i][rest] = max(dp[i - 1][rest - weight[i]] + value[i], dp[i - 1][rest])
*在完全背包里，考虑过i后还可以再考虑，因为第i个物品有无限多个，所以不考虑i的情况是dp[i - 1][rest]与上面一样，而考虑i的情况是dp[i][rest - weight[i]] + value[i]
*所以dp[i][rest] = max(dp[i][rest - weight[i]] + value[i], dp[i - 1][rest]);
*/
void fill_dp_complete(int target, vector<int>& value, vector<int>& weight, vector<int>& dp){
    int n = value.size();
    for (int i = 0; i < n; i++){//我们一行一行往下卷
        for (int j = 0; j <= target; j++){//一次算一行并往下卷一行
        if (weight[i] <= j)
            dp[j] = max(dp[j], dp[j - weight[i]] + value[i]);//当上一行卷下来，还没有进行max比较的时候，dp[j]不就是上一行的值吗？
            //那为什么j要从左边开始，也就是为什么j一定要++？原因与上面类似，因为我们当前值的计算用到了左边的值dp[i][rest - weight[i]]，也就是最新的左边的值
            //所以这里要从左边开始，先把dp[i]算出来，让后面来用
            //这又时就需要我们实现将第0行和0列的值先手动指定一下
        }
    }
}