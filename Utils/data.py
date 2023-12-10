import pandas as pd
import os


def load_data(data_dir: str, dtypes: dict, sep=';') -> list[pd.DataFrame]:
    """Loads all .csv files into a pandas.Dataframe and returns them as a list"""
    dataframes = []
    for root, _, files in os.walk(data_dir):
        for filename in files:
            if filename.endswith('.csv'):
                filepath = os.path.join(root, filename)
                df = pd.read_csv(filepath, sep=sep, index_col=False, dtype=dtypes)
                dataframes.append(df)
    return dataframes


def average_dataframe(dataframes: list[pd.DataFrame], columns: list[str]) -> pd.DataFrame:
    """Averages the dataframes provided on columns"""
    for i in range(len(dataframes)):
        dataframes[i] = dataframes[i][columns]
    df_avg = dataframes[0].copy()
    for dataframe in dataframes[1:]:
        df_avg.add(dataframe)
    df_avg.div(len(dataframes))
    return df_avg

