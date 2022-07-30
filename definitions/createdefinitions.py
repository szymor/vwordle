import json
import sqlite3
def createTable():
    con = sqlite3.connect('definitions.db')
    cur = con.cursor()
    sql = """
        create table words(id integer primary key autoincrement, word text);
        create table synonyms(synonym_id integer primary key autoincrement,synonym text,word_id integer,foreign key(word_id) references words(id));
        create table speech_part(speechpart_id integer not null primary key,speech_part text);
        create table definitions(definitions_id integer primary key autoincrement, definition text, word_id integer,speech_part_id integer,foreign key(word_id) references words(id),
        foreign key(speech_part_id) references speech_part(speechpart_id));
        insert into speech_part(speechpart_id,speech_part) values(1, 'verb');
        insert into speech_part(speechpart_id,speech_part) values(2, 'noun');
        insert into speech_part(speechpart_id,speech_part) values(3, 'pronoun');
        insert into speech_part(speechpart_id,speech_part) values(4, 'adjective');
        insert into speech_part(speechpart_id,speech_part) values(5, 'adverb');
        insert into speech_part(speechpart_id,speech_part) values(6, 'preposition');
        insert into speech_part(speechpart_id,speech_part) values(7, 'conjunction');
        insert into speech_part(speechpart_id,speech_part) values(8, 'interjection');
    """
    cur.executescript(sql)
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
                    word = data[f.strip("\n")]['word']
                    cur.execute(f"insert into words(word) values ('{word}');")
                    con.commit()
                    for meaning in data[f.strip('\n')]['meanings']:
                        cur.execute(f"select speechpart_id from speech_part where speech_part='{meaning['speech_part']}';")
                        speech_part_id = cur.fetchone()
                        cur.execute(f"select id from words where word='{word}'")
                        word_id = cur.fetchone()
                        print(word_id)
                        print("eror")
                        sql = f"insert into definitions(definition,word_id,speech_part_id) values('{meaning['def']}',{word_id[0]},{speech_part_id[0]});"
                        cur.execute(sql)
                        con.commit()
                        if 'synonyms' in meaning:
                            for synonym in meaning['synonyms']:
                                cur.execute(f"insert into synonyms(synonym, word_id) values ('{synonym}', {word_id[0]})")
                                con.commit()
                except:
                    print("error")
                con.commit()
        con.close()
                
if __name__ == "__main__":
    createTable()
    dict_words_files  =  ['5.txt', '6.txt', '7.txt', '8.txt'] # replace with your word dictionary
    for f in dict_words_files:
        createDefinition(f)