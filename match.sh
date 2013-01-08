for samplefile in ./samples/*;
  do 
    echo "Searching for $samplefile in database. First line is best match: "
    for databasefile in ./signatures/*;
      do 
        ./ber $samplefile $databasefile 
        echo "$databasefile " 
    done | sort -n
done 
