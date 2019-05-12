import requests
from time import sleep
class BotHandler():
    def __init__(self):
        self.url = "https://api.telegram.org/bot843398054:AAHAyuATGrxj0pkOYE-3werDPg3Pl24HHh4/"
        self.general_offset = None


    def get_updates(self):
        method = 'getUpdates'
        params = {'offset': self.general_offset}
        resp = requests.get(self.url + method, params)
        result_json = resp.json()['result']
        real_result = []
        if len(result_json) > 0:
            real_result = []
            for element in result_json:
                if self.general_offset is None or element['update_id'] >= self.general_offset:
                    real_result += [element]
        if len(real_result) > 0:
            self.general_offset = result_json[-1]['update_id'] + 1
        return result_json

    def send_message(self, chat_id, text):
        params = {'chat_id': chat_id, 'text': text}
        method = 'sendMessage'
        resp = requests.post(self.url + method, params)
        return resp

def throw(b, id):
    b.send_message(id, 'Wrong request')

def used_login(b, id):
    b.send_message(id, 'Your login`s been already used')

def success(b, id):
    b.send_message(id, 'Your login`s been successfully registered')

def message_result(b, id, result, duration, time):
    date = str(time)[:10]
    tm = str(time)[11:19]
    mess = "Your commit made in " + tm + ' UTC on ' + date
    mess += ' has ' + result
    mess += ' consuming ' + str(duration) + ' seconds'
    b.send_message(id, mess)

def main():
    a = dict()
    last_commit = dict()
    keys = dict()
    chats = dict()
    used_numbers = dict()
    b = BotHandler()
    while True:
        sleep(2)
        line = b.get_updates()
        if len(line) > 0:
            for element in line:
                id = element['message']['chat']['id']
                string = element['message']['text']
                ln = string.split()
                if len(ln) != 6 or ln[0] != '/register':
                    throw(b, id)
                    continue
                github_login = ln[1]
                project_owner = ln[2]
                project_name = ln[3]
                key = ln[4]
                commit_index = ln[5]
                url = 'https://api.travis-ci.org/repo/' + project_owner + '%2F' + project_name
                key_string = 'token ' + key
                resp = requests.get(url, headers={'Travis-API-Version': '3', 'Authorization': key_string, 'User-Agent': 'API Explorer'}).json()
                if resp['@type'] == 'error':
                    throw(b, id)
                    continue
                if github_login in a:
                    used_login(b, id)
                    continue
                success(b, id)
                a[github_login] = resp['id']
                last_commit[github_login] = commit_index
                keys[github_login] = key_string
                chats[github_login] = id
                ns = set()
                used_numbers[github_login] = ns
        for login in a:
            repo = a[login]
            interested_list = []
            url = 'https://api.travis-ci.org/repo/' + str(repo) + '/builds?limit=25'
            resp = requests.get(url, headers={'Travis-API-Version': '3', 'Authorization': keys[login], 'User-Agent': 'API Explorer'}).json()
            builds = resp['builds']
            for limit in range(25):
                if len(builds) <= limit:
                    break
                interesting_build = builds[limit]
                number = interesting_build['number']
                if (number <= last_commit[login]):
                    continue
                if (number in used_numbers[login]):
                    continue
                if interesting_build['created_by']['login'] != login:
                    continue
                if interesting_build['duration'] is None:
                    continue
                used_numbers[login].add(number)
                interested_list += [interesting_build]
            interested_list = interested_list[::-1]
            for interesting_build in interested_list:
                message_result(b, chats[login], interesting_build['state'], interesting_build['duration'], interesting_build['started_at'])

main()
