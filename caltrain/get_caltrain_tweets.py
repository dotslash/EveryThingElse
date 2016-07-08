import tweepy
import json
import sys
from colorama import Fore, Back, Style
import re
import argparse
import pytz

local_tz = pytz.timezone('America/Los_Angeles')


def bright_green(inp):
    return Style.BRIGHT + Fore.GREEN + inp + Fore.RESET + Style.RESET_ALL

def bright_cyan(inp):
    return Style.BRIGHT + Fore.CYAN + inp + Fore.RESET + Style.RESET_ALL

def bright_red(inp):
    return Style.BRIGHT + Fore.RED + inp + Fore.RESET + Style.RESET_ALL


def transform(inp):
    text = re.sub(r'(?<!\w)(([NS]B)?\s*\d{3})(?!\w)', bright_green(r'\1'), inp.text, flags = re.IGNORECASE)
    text = re.sub('(\d+\s+?mins?(\s*late)?)', bright_red(r'\1'), text, flags = re.IGNORECASE)
    inp.new_text = text
    return (text != inp.text, text + ' -- ' + extract_time(inp))

def extract_time(status): 
    dt = status.created_at
    local_dt = dt.replace(tzinfo=pytz.utc).astimezone(local_tz)
    ret = local_tz.normalize(local_dt)
    return bright_cyan(ret.strftime('%b %d %H:%M'))

def main(args, all=False, account='Caltrain'):
    auth = tweepy.OAuthHandler(args.consumer_key, args.consumer_secret)
    auth.secure = True
    auth.set_access_token(args.access_token, args.access_token_secret)

    api = tweepy.API(auth)

    tweets = [transform(status) for status in api.user_timeline(id=account)]
    p1 = [status for change, status in tweets if change]
    p2 = [status for change, status in tweets if not change]
    print('\n--------------------------------\n'.join(p1))
    if all:
        print("===========================================================")
        print('\n--------------------------------\n'.join(p2))


'''
texts = [
 "#NB277 is 11 mins late out of San Bruno. #Caltrain\n",
 "254-269-386 will run with a 5 car Gallery Set instead of a 6 car Bombardier. #Caltrain\n",
 "385 -21min late @ BWY\n287 -14min BUR\n289 -24min PAL\n191 -35min SCL\n#Caltrain"
 ]

for text in texts: print transform(text)
'''
parser = argparse.ArgumentParser(description='Caltrain tweets')

parser.add_argument('-ck', help="Consumer Key", 
                    dest="consumer_key", required=True)
parser.add_argument('-cs', help="Consumer Secret", 
                    dest="consumer_secret", required=True)
parser.add_argument('-at', help="Access token", 
                    dest="access_token", required=True)
parser.add_argument('-ats', help="Access token Secret", 
                    dest="access_token_secret", required=True)

parser.add_argument('-u', help="User Account", dest="account",
                    type=str, default='Caltrain')
parser.add_argument('-a', help="All tweets", dest="all",
                    type=bool, default=False)

args = parser.parse_args()
main(args, all=args.all, account=args.account)
