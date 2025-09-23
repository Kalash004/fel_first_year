import os
class Corpus():
    def __init__(self,path_to_dir):
        self.path = path_to_dir
        self.file_names = os.listdir(self.path)
        
    def emails(self):
        for f_name in self.file_names:
            if f_name[0] == "!":
                continue
            data = self.__read_file(os.path.join(self.path,f_name))
            yield (f_name, data)
            
            
    def __read_file(self, path):
        with open(path, 'r', encoding="utf-8") as f:
            file_data = f.read()
            return file_data