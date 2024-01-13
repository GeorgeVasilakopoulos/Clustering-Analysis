import pandas as pd

df = pd.read_csv('/mnt/c/Users/10geo/Documents/GitHub/Project/src/autoencoder/encoder/results.csv', sep='\t')

df.drop(columns=['Depth'], inplace=True)
df['Score'] = df['AF'] + df['MAF'] - 2 * df['Accuracy'] - df['Neibs']

df_score = df.sort_values(by='Score', ascending=True)
df_maf = df.sort_values(by='MAF', ascending=True)

print(df_score.head())
