import itertools as it
import pprint
import time

pp = pprint.PrettyPrinter(indent=4)

def my_merge(a,b):
	for i in b:
		a[i].update(b[i])
	return a

def planar(n):
	face_dict = {}
	def add_vertices(OF, N):
		if tuple(OF) in face_dict:
			#print "here"
			return face_dict[tuple(OF)]
	
		n = max(OF)
		if n+1 == N:
			return [{ i:{} for i in range(N)}] 
	
		#print "there"
		n2 = len(OF)
		extensions = []
		for i in range(n2):
			OF2 = OF[i:] + OF[:i]
			for j in range(n2):
				OF3 = tuple([OF2[0]] + [n+1] + list(OF2[j:]))
				combos = {():1}
				H = {l:{} for l in range(n+2)}
				for l in range(len(OF2)-1):
					H[OF2[l]][OF2[l+1]] =1
					H[OF2[l+1]][OF2[l]] =1
				H[OF2[-1]][OF2[0]] =1
				H[OF2[0]][OF2[-1]] =1
				for l in range(j+1):
					H[n+1][OF2[l]] = 1
					H[OF2[l]][n+1] = 1
				temp =  add_vertices(OF3, N)
				[ my_merge(l,H) for l in temp]
				extensions += temp
				extensions += [H]
		temp2 = set([ tuple([tuple(j.keys()) for j in i.values()]) for i in extensions])
		tuple_ext = [ {j:{k:1 for k in i[j]} for j in range(len(i))} for i in temp2]
		for k in range(len(OF)):
			face_dict[tuple(OF[k:] + OF[:k])] = tuple_ext
		return face_dict[tuple(OF)] 
	
	# initiate outer face i.e. edge twice
	OF = ((1),(0))
	add_vertices(OF, n)
	return face_dict[OF]


start = time.time()

planar_graphs = planar(7)
#tup_plan = [tuple([tuple(j.keys()) for j in i.values()]) for i in planar_graphs]
#pp.pprint(tup_plan)

end = time.time()
print(end - start)














