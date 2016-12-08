import itertools as it
import pprint
import time

pp = pprint.PrettyPrinter(indent=4)

def planar(N):
	face_dict = {}
	def add_vertices(OF, N):
		if tuple(OF) in face_dict:
			#print "here"
			return face_dict[tuple(OF)]
	
		n = max(OF)
		if n+1 == N:
			return [tuple([ tuple([0 for j in range(N)]) for i in range(N)])] 
	
		#print "there"
		n2 = len(OF)
		extensions = []
		for i in range(n2):
			OF2 = OF[i:] + OF[:i]
			for j in range(n2):
				OF3 = tuple([OF2[0]] + [n+1] + list(OF2[j:]))
				combos = {():1}
				[ combos.update({l:1 for l in it.combinations(OF2[1:j],k)}) for k in range(1,j) ]
				for k in combos:
					H = [[ 0 for i in range(N)] for l in range(N)]
					H[n+1][OF2[0]] = 1
					H[OF2[0]][n+1] = 1
					H[n+1][OF2[j]] = 1
					H[OF2[j]][n+1] = 1
					for l in range(len(OF2)-1):
						H[OF2[l]][OF2[l+1]] = 1
						H[OF2[l+1]][OF2[l]] = 1
					H[OF2[-1]][OF2[0]] = 1
					H[OF2[0]][OF2[-1]] = 1
					for l in k:
						H[n+1][l] = 1
						H[l][n+1] = 1
					H = tuple([tuple(x) for x in H])
					temp = add_vertices(OF3, N)
					
					extensions += [ tuple([tuple([l[x][y] if H[x][y] == 1 else l[x][y] for y in range(N)]) for x in range(N)]) for l in temp]
					extensions += [H]
		temp2 = list(set(extensions))
		for k in range(len(OF)):
			face_dict[tuple(OF[k:] + OF[:k])] = temp2
		return face_dict[tuple(OF)] 
	
	# initiate outer face i.e. edge twice
	OF = ((1),(0))
	add_vertices(OF, N)
	return face_dict[OF]


start = time.time()

planar_graphs = planar(7)
#tup_plan = [tuple([tuple(j.keys()) for j in i.values()]) for i in planar_graphs]
#pp.pprint(tup_plan)

end = time.time()
print(end - start)











