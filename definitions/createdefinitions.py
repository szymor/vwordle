import json
import sqlite3


def createTable():
    con = sqlite3.connect('definitions.db')
    cur = con.cursor()
    cur.execute("CREATE TABLE definitions(word text,def1 text,speech_part1 text,def2 text,speech_part2 text);")
    con.commit()
    con.close()
def createDefinition(file):
    con = sqlite3.connect('definitions.db')
    cur = con.cursor()
    with open(file, encoding="UTF-8") as file:
        for f in file:
            ''' first letter of word identificates which json file should be open, because
                first letter of word identificates which json file should be open,
                 because each file has only definitions for words which starting with letter in filename'''
            with open(f[0]+".json", encoding="UTF-8") as source: 
                data = json.load(source)
                try:
                    def1 = None
                    def2 = None
                    speech_part1 = None
                    speech_part2 = None
                    word = data[f.strip("\n")]['word']
                    for i in range(0,2):
                        if i == 0:
                            def1 = data[f.strip('\n')]['meanings'][0]['def']
                            speech_part1 = data[f.strip('\n')]['meanings'][0]['speech_part']
                            #print(data[f.strip('\n')]['meanings'][0]['def'])
                        if i == 1:
                            try:
                                def2 = data[f.strip('\n')]['meanings'][1]['def']
                                speech_part2 = data[f.strip('\n')]['meanings'][1]['speech_part']
                            except:
                                pass
                    cur.execute(f"INSERT INTO definitions Values('{word}','{def1}','{speech_part1}','{def2}', '{speech_part2}');")
                except:
                    pass
                con.commit()
        con.close()
                
if __name__ == "__main__":
    createTable()
    dict_words_files  =  ['5.txt', '6.txt', '7.txt', '8.txt'] # replace with your word dictionary
    for f in dict_words_files:
        createDefinition(f)