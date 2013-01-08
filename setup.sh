for file in ./tracks/*; 
  do ./extract $file ./signatures/$(basename $file.signature | sed s/"\.wav"//); 
done
for file in ./sample-tracks/*;
  do ./extract $file ./sample-signatures/$(basename $file.signature | sed s/"\.wav"//);
done
