from __future__ import division
import numpy as np

# using h5py
#import h5py as h5pi

#test = h5pi.File('name_of_h5.h5',mode='r',driver='core')
#test.keys()[:] # this contains all the data files' name inside hdf5

# defining the grid
Nx,Ny,Nz =100, 100, 100

# Total time steps
tEnd = 10

# Data dumping step
nOutput = 10

# Not taking initial points
waittime = 3400

filename = 'test.xmf'
f = open(filename, 'w')

# Header for xml file
f.write('''<?xml version="1.0" ?>
<!DOCTYPE Xdmf SYSTEM "Xdmf.dtd" []>
<Xdmf Version="2.0">
<Domain>
<Grid Name="Box" GridType="Collection" CollectionType="Temporal">
''')

# loop over the attributes name written using time
t = 0
frameN = 0 # For time sequence 
while t <= tEnd :
    t = t + 1; 
 #   if( np.mod(t, nOutput) == 0 and t > waittime):
    if(True):
        # Naming datasets 
   #     dataSetName1 = 'test.h5:/S_%.8d'%(t)
        dataSetName2 = 'test.h:/Field/%d./E0'%(t)

        # at individual time write the time independent Box grid. is it overdoing?
        f.write('''
        <!-- time step -->
        <Grid Name="Box %d" GridType="Uniform"> # 
        <Topology TopologyType="3DCoRectMesh" Dimensions="%d %d %d"/>
        <Geometry GeometryType="ORIGIN_DXDYDZ">
           <DataItem DataType="Float" Dimensions="3" Format="XML">0.0 0.0 0.0</DataItem>
           <DataItem DataType="Float" Dimensions="3" Format="XML">1.0 1.0 1.0</DataItem>
        </Geometry>
        <Time Value="%d" />
        '''%(frameN, Nx, Ny, Nz, frameN))

        # Second Attribute
        f.write('''\n
        <Attribute Name="N" AttributeType="Scalar" Center="Node">
        <DataItem Dimensions="%d %d %d" NumberType="Float" Precision="8"
        Format="HDF"> %s
        </DataItem>
        </Attribute>
   
        </Grid>\n'''%(Nx, Ny, Nz, dataSetName2))
        frameN +=1

# End the xmf file
f.write('''
   </Grid>
</Domain>
</Xdmf>
''')