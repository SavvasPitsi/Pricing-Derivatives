from pathlib import Path
import yfinance as yf
import pandas as pd
from datetime import datetime

# function definitions

def fetch_rates():

    date = datetime.today().strftime('%Y%m')
    df = pd.read_csv("https://home.treasury.gov/resource-center/data-chart-center/interest-rates/"+
                     "daily-treasury-rates.csv/all/"+date+"?type=daily_treasury_yield_curve"+
                     "&field_tdr_date_value_month="+date+"&page&_format=csv")

    return df

# First you need to make sure you have the latest the yield curve data

if Path('rates.csv').is_file():   #check if the file exists
    df = pd.read_csv('rates.csv')
    df.sort_values(by='Date')

    if datetime.strptime(df['Date'].iloc[-1], '%m/%d/%Y').date() < datetime.today().date():
        # if the most recent entry is older than today, fetch new data
          
        df = fetch_rates()
        df.sort_values(by='Date')

        if datetime.strptime(df['Date'].iloc[-1], '%m/%d/%Y').date() < datetime.today().date():
            # if the new data are older than today, create a new line with the current date and the most recent data
            # this makes sure that we fetch data once per day max

            df.loc[len(df)] = df.iloc[-1]
            df.iat[-1,0] = datetime.today().strftime('%m/%d/%Y')

        
        df.to_csv('rates.csv', index=False)
else:
    df = fetch_rates()
    df.sort_values(by='Date')

    if datetime.strptime(df['Date'].iloc[-1], '%m/%d/%Y').date() < datetime.today().date():
        # if the new data are older than today, create a new line with the current date and the most recent data
        # this makes sure that we fetch data once per day max

        df.loc[len(df)] = df.iloc[-1]
        df.iat[-1,0] = datetime.today().strftime('%m/%d/%Y')

    df.to_csv('rates.csv', index=False)

# create the yields.csv cleaned file, with only a single row
df.iloc[[-1]].dropna(axis=1).to_csv('yields.csv', index=False)