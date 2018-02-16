Hacky quick get-measurements procedure:

// 0. Open tsv_file
// 1. Build core graph from the 30% of tsv_file
// (which is equivalent to calling update_from_tsv(start, end, *tsv_file) with
// start=0 and end= line which marks 30%dataset)
// 2. Call graph algorithm function and compute; update vertex states
// 3. Call update_from_tsv(start, end, *tsv_file);
// 3.1 Go through lines start to end
// 3.2 Parse each line to extract vertices u,v
// 3.3 Call add_edge(u,v) for each line to apply an update to the existing graph
// 4. end line becomes start line for next iteration of update_from_tsv()
// 5. Repeat from 2 until end of the dataset.