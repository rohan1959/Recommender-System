#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

#define M 5000
#define N 5000

struct node
{
	int user, item, rating;
};


vector<vector<int> > matrix(M + 1, vector<int>(N + 1, 0));
vector<vector<double> > temp(M + 1, vector<double>(N + 1, 0));
vector<vector<double> > sim(N + 1, vector<double>(N + 1, 0));
vector<vector<int> > preTable(N+1);

vector<double> itemAvgRatings(N + 1);
vector<double> userAvgRatings(M + 1);

int main()
{
	
	freopen("train_all_txt.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int user, item, rating;

	int m = 0;
	int n = 0;

	vector<node> vals;
	while (scanf("%d %d %d", &user, &item, &rating) != EOF)
	{
		m = max(m, user);
		n = max(n, item);
		node temp;
		temp.user = user;
		temp.item = item;
		temp.rating = rating;
		vals.push_back(temp);
	}
	


	for (int i = 0; i < vals.size(); i++)
	{
		matrix[vals[i].user][vals[i].item] = vals[i].rating;
		temp[vals[i].user][vals[i].item] = vals[i].rating;
	}

	for (int i = 1; i <= m; i++)
	{
		double cnt, sum;
		sum = cnt = 0;
		for (int j = 1; j <= n; j++)
		{
			if (matrix[i][j] > 0)
			{
				cnt++;
				sum += matrix[i][j];
			}
		}
		if (cnt != 0)
			userAvgRatings[i] = sum / cnt;
		else
			userAvgRatings[i] = 0;
	}
	
	double sumTotal = 0;
	double cntTotal = 0;
	double avgItemRating = 0;

	for (int i = 1; i <= n; i++)
	{
		double cnt = 0;
		double sum = 0;
		for (int j = 1; j <= m; j++)
		{
			if(matrix[j][i]!=0)
				sum += matrix[j][i],cnt++;
		}
		if (cnt != 0)
			itemAvgRatings[i] = sum / cnt;
		else
			itemAvgRatings[i] = 0;

		for (int j = 1; j <= m; j++)
		{
			if (matrix[j][i] != 0)
				temp[j][i] -= itemAvgRatings[i];
		}

		sumTotal += sum;
		cntTotal += cnt;

	}

	avgItemRating = sumTotal / cntTotal;

	for (int i = 1; i <= n; i++)
	{
		for (int j = i+1; j <= n; j++)
		{
			double sum = 0;
			double s1 = 0, s2 = 0;
			for (int k = 1; k <= m; k++)
			{
				sum += temp[k][i] * temp[k][j];
				s1 += temp[k][i] * temp[k][i];
				s2 += temp[k][j] * temp[k][j];
			}
			if (s1*s2 == 0)
				sim[i][j] = -1;
			else
				sim[i][j] = sum / sqrt(s1*s2);
			sim[j][i] = sim[i][j];
		}
	}

	for (int i = 1; i <= n; i++)
	{
		for (int j = i+1; j <= n; j++)
		{
			if (sim[i][j] >= 0)
			{
				preTable[i].push_back(j), preTable[j].push_back(i);
			}
					
		}
	}

	for (int i = 1; i <= m; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			double baseline = (avgItemRating + userAvgRatings[i] + itemAvgRatings[j] - 2 * avgItemRating);
			if (matrix[i][j] == 0)
			{
				double sum = 0;
				double den = 0;
				for (int k = 0; k < preTable[j].size(); k++)
				{
					int p = preTable[j][k];
					if (sim[j][p] >= 0 && matrix[i][p] > 0)
					{
						sum += sim[j][p] * (matrix[i][p] - (avgItemRating+userAvgRatings[i]+itemAvgRatings[p] - 2 * avgItemRating));
						den += sim[j][p];
					}
				}
				if (den != 0)
					temp[i][j] = baseline + (sum / den);
				else
					temp[i][j] = baseline;
				if (temp[i][j] < 1.0)
					temp[i][j] = 1;
				int pr = (int)(temp[i][j] + 0.5);
				printf("%d %d %d\n", i, j, pr);
			}
			else
				printf("%d %d %d\n", i, j, matrix[i][j]);
		}
	}

	return 0;
}