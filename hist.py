#!/home/newlord/anaconda3/bin/python3

import sys
import os
import glob
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns


def plot():
	df=pd.read_csv("./ntpc.csv", delimiter=',', header=None)
	#print (df)

	#df_miss = df.loc[ (df[0]==1) & (df[1]<36000) & (df[1]>30000)]
	#df_hit = df.loc[ (df[0]==0) & (df[1]<36000) & (df[1]>30000)]
	df_miss = df.loc[ (df[0]==2) & (df[1]<36000) & (df[1]>30000)]
	df_hit = df.loc[ (df[0]==1) & (df[1]<36000) & (df[1]>30000)]
	df_miss = df_miss.reset_index()
	df_hit = df_hit.reset_index()

	df_miss=df_miss.rename(columns={1: "miss"})
	df_hit=df_hit.rename(columns={1: "hit"})

	avg_miss=df_miss["miss"].mean()
	avg_hit=df_hit["hit"].mean()

	print("miss={0}, hit={1}, diff={2}".format(avg_miss, avg_hit, avg_miss-avg_hit))



	#df_result=df_miss[["miss"]]
	#df_result=df_result.join(df_hit[["hit"]])
	#print(df_result)

	fig, axe = plt.subplots(nrows=1, ncols=1, figsize=(14,4))
	
	sns.distplot(df_miss["miss"], ax=axe, hist=False, kde=True, label='miss')
	sns.distplot(df_hit["hit"], ax=axe, hist=False, kde=True, label='hit')
	plt.show()

plot()
