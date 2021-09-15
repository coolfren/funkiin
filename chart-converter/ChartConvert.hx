import sys.io.File;
import sys.FileSystem;
import haxe.Json;
using StringTools;

class ChartConvert {
  static function main() {
    if(FileSystem.exists("./charts")){
        var files = FileSystem.readDirectory("./charts");
            for(file in files){
                var chart = File.getContent("./charts/"+file);
                var parsed = Json.parse(chart);
                var conchart = '{';
                for(i in 0...parsed.song.notes.length){
                    for(r in 0...parsed.song.notes[i].sectionNotes.length){
                            conchart +='{';
                            for(b in 0...parsed.song.notes[i].sectionNotes[r].length){
                            conchart += parsed.song.notes[i].sectionNotes[r][b];
                            conchart += ','; //if(b!=parsed.song.notes[i].sectionNotes[r].length-1) 
                            }
                            conchart += parsed.song.notes[i].mustHitSection;
                            conchart +='},';
                        }
                }
                conchart += '{0, 0, 0, false}}';
                var template = File.getContent("template.h");
                template = template.replace("chrtfile", conchart).replace("chart", file.toLowerCase().replace(".json", "")).replace("CHART_H", file.replace(".json", "_h").toUpperCase());
                File.saveContent("./output/"+file.replace(".json", ".h"), template);
                trace("Chart converted: "+file);
            }
        
    }else{
        trace("The charts directory does not exist!");
        Sys.exit(1);
    }
  }
}
/*WIP
          conchart +='{';
          for(b in 0...parsed.song.notes[i].sectionNotes[r].length){
            conchart += parsed.song.notes[i].sectionNotes[r][b];
            if(b!=parsed.song.notes[i].sectionNotes[r].length-1) conchart += ',';
          }
          conchart +='},';
          */