bl_info = {
    "name": "Pico3D Exporter",
    "blender": (2, 80, 0),
    "category": "Import-Export",
}

import bpy
from os import path

class Pico3dPanel(bpy.types.Panel):
    bl_idname = 'VIEW3D_PT_pico3d_panel'
    bl_label = 'Pico3D Export'
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    
    def draw(self, context):
        
        
        col = self.layout.column()
        for (prop_name, _) in PROPS:
            row = col.row()
            row.prop(context.scene, prop_name)
            
        
        col.operator('object.export_pico3d', text='EXPORT')
        
        
        self.layout.label(text='')
        self.layout.label(text='Advanced:')
        
        col = self.layout.column()
        for (prop_name, _) in PROPS_ADVANCED:
            row = col.row()
            row.prop(context.scene, prop_name)
        
        
        
PROPS = [
    ('world_size_x', bpy.props.IntProperty(name='World Size X', default=12)),
    ('world_size_y', bpy.props.IntProperty(name='World Size Y', default=12)),
    ('export_single_model', bpy.props.BoolProperty(name='Export Single Model', default=False)),
    ('meshname', bpy.props.StringProperty(name='Mesh', default='Cube')),
    ('modelname', bpy.props.StringProperty(name='Model', default='testmodel')),
    ('file_path', bpy.props.StringProperty(name='Path', default='', subtype='FILE_PATH')),
]


PROPS_ADVANCED = [
    ('lod0_triangle_limit', bpy.props.IntProperty(name='LoD 0 Triangle Limit', default=255)),
    ('lod1_triangle_limit', bpy.props.IntProperty(name='LoD 1 Triangle Limit', default=128)),
    ('scaling_factor', bpy.props.IntProperty(name='Scaling Factor', default=1)),
    #('fixed_point', bpy.props.BoolProperty(name='Fixed Point Math', default=True)),
    ('fixed_point_factor', bpy.props.IntProperty(name='Fixed Point Factor', default=1024)),
    ('fixed_point_16', bpy.props.BoolProperty(name='16-bit Vertex Ints', default=True)),
    ('store_ram', bpy.props.BoolProperty(name='Store in RAM', default=True)),
]



class Pico3dExport(bpy.types.Operator):
    """Exports Meshes to .h file"""      # Use this as a tooltip for menu items and buttons.
    bl_idname = "object.export_pico3d"        # Unique identifier for buttons and menu items to reference.
    bl_label = "Export to Pico3D"         # Display name in the interface.
    #bl_options = {'REGISTER', 'UNDO'}  # Enable undo for the operator.

    textures = []
    texture_list = ''
    texture_data = ''

    def execute(self, context):        # execute() is called when running the operator.

        # The original script
        scene = context.scene
        
        
        export_single = context.scene.export_single_model
        
        
        #if there is no path yet, ask user to fill in path
        file_path = context.scene.file_path
        
        if (file_path == ''):
            print('No file path specified, dumping output to console:\n')
            
        file_output = ''
        
        self.textures = []
        
        self.texture_list = ''
        
        # Either export a single model
        if export_single:
            
            meshname = context.scene.meshname
            
            if meshname in bpy.data.meshes:
                
                modelname = context.scene.modelname
                mesh = bpy.data.meshes[meshname]
                
                

                output = self.generate_vertices(context, mesh, modelname, 1000, False) + '\n'
                
                #output += self.generate_textures()
                
                
                if (file_path == ''):
                    print(output)
                else:
                    f = open(file_path, "w")
                    f.write(output)
                    f.close()
                
            else:
                print('Unable to find mesh using given name')
        
        #Otherwise proceed with a complete chunk export
        else:
            
            world_size_x = context.scene.world_size_x
            world_size_y = context.scene.world_size_y

            header_output = ''
            src_output = f'#include "{path.basename(file_path)}.h"\n#include "engine/render_globals.h"\n#include "engine/chunk_globals.h"\n'

            # world size
            header_output += '#define WORLD_SIZE_X ' + str(world_size_x) + '\n#define WORLD_SIZE_Y ' + str(world_size_y) + '\n'

            #First do lod0
            max_triangles = context.scene.lod0_triangle_limit
            
            chunk_decl, chunk_impl = self.export_chunks(context, 0, max_triangles)
            header_output += chunk_decl
            src_output += chunk_impl + '\n'

            #Then do lod1
            max_triangles = context.scene.lod1_triangle_limit

            chunk_decl, chunk_impl = self.export_chunks(context, 1, max_triangles)
            header_output += chunk_decl
            src_output += chunk_impl + '\n'

            # textures and lights
            
            textures_decl, textures_impl = self.generate_textures()
            
            
            lights_decl, lights_impl = self.generate_lights(context)

            header_output += textures_decl + '\n' + lights_decl
            src_output += textures_impl + '\n' +lights_impl
            
            #print(triangle_output)
            #print(chunk_output)
            
            if (file_path == ''):
                print(header_output)
                print(src_output)
            else:
                f = open(file_path + '.h', "w")
                f.write(header_output)
                f.close()

                f = open(file_path + '.cpp', "w")
                f.write(src_output)
                f.close()

                
            

        return {'FINISHED'}            # Lets Blender know the operator finished successfully.








    def generate_vertices(self, context, mesh, modelname, max_triangles, is_chunk):
        
        scaling_factor = context.scene.scaling_factor
        model_offset_z = 0 # to push model back
        fixed_point_factor = context.scene.fixed_point_factor
        store_ram = context.scene.store_ram
        reduced_range = context.scene.fixed_point_16
        
        
        # We need the triangles
        mesh.calc_loop_triangles()
        i = 0;
        triangles = 0;
        vertices = ""
        
        #Vertex Colors
        if 'Col' in mesh.vertex_colors:
            colors = mesh.vertex_colors['Col']
        else:
            #print("Missing Color (Use Vertex Paint) for mesh: ", modelname)
            colors = False
        
        
            
        
        #Loop through each triangle
        for tri in mesh.loop_triangles:
            
            
            if (triangles == 0):
                vertices += "{"
            else:
                vertices += ", {"
            
            x = mesh.vertices[tri.vertices[0]].co.x * scaling_factor
            y = mesh.vertices[tri.vertices[0]].co.z * scaling_factor
            z = (-mesh.vertices[tri.vertices[0]].co.y - model_offset_z) * scaling_factor
            
            #Convert vertex points to fixed point
            x = int(x * fixed_point_factor)
            y = int(y * fixed_point_factor)
            z = int(z * fixed_point_factor)
        
            vertices += "{ " + str(x) + ", " + str(y) + ", " + str(z) + "},"
            
            
            
            x = mesh.vertices[tri.vertices[1]].co.x * scaling_factor
            y = mesh.vertices[tri.vertices[1]].co.z * scaling_factor
            z = (-mesh.vertices[tri.vertices[1]].co.y - model_offset_z) * scaling_factor
            
            #Convert vertex points to fixed point
            x = int(x * fixed_point_factor)
            y = int(y * fixed_point_factor)
            z = int(z * fixed_point_factor)
            
            vertices += "{ " + str(x) + ", " + str(y) + ", " + str(z) + "},"
            
            
            
            x = mesh.vertices[tri.vertices[2]].co.x * scaling_factor
            y = mesh.vertices[tri.vertices[2]].co.z * scaling_factor
            z = (-mesh.vertices[tri.vertices[2]].co.y - model_offset_z)  * scaling_factor
            
            #Convert vertex points to fixed point
            x = int(x * fixed_point_factor)
            y = int(y * fixed_point_factor)
            z = int(z * fixed_point_factor)
            
            vertices += "{ " + str(x) + ", " + str(y) + ", " + str(z) + "}, "
            
            
            
            
            
            texture = False
            glow = False #if a triangle ignores dynamic lighting
            
            if len(mesh.materials) > 0:
                for n in mesh.materials[tri.material_index].node_tree.nodes:
                    if n.type == 'TEX_IMAGE':
                        #texture_list += [n.image]
                        #print(mesh.name,'uses',n.image.name,'saved at',n.image.filepath)
                        texture = n.image
                    #also check if material is a glow material
                    if (mesh.materials[tri.material_index].name.find('glow') != -1):
                        #print ('glow material found')
                        glow = True
                       
            
            #if we have a texture, we export using uv maps and texture shaders
            if texture is not False:
                
                
                #check if texture is not already part of the list
                if n.image.filepath not in self.textures:
                    self.textures.append(n.image.filepath)
                    #print('added to texture list')
                    new_index = self.textures.index(n.image.filepath)
                    new_size = n.image.size[0]
                    
                    #we have to add the texture to the texture_chunk_list
                    if (len(self.texture_list) > 0):
                        self.texture_list += ',\n'
                    self.texture_list += '{' + str(new_size) + ', 0, 0, texture' + str(new_index) + '}'
                    
                    #once in the chunk list, we have to save the raw data
                    self.texture_data += '//' + n.image.filepath + '\n'
                    self.texture_data += 'const color_t texture' + str(new_index) + '[' + str(new_size * new_size) + '] = {'
                    
                    counter = 0;
                    r = 0;
                    g = 0;
                    b = 0;
                    
                    for pixel in n.image.pixels:
                        #print(pixel)
                        if (counter % 4 == 0): #R
                            r = hex(int(pixel * 15))[2:]
                            #self.texture_data += "0x" + hex(int(pixel * 15))[2:]
                        elif (counter % 4 == 1): #G
                            g = hex(int(pixel * 15))[2:]
                            #self.texture_data += hex(int(pixel * 15))[2:] + '0'
                        elif (counter % 4 == 2): #B
                            b = hex(int(pixel * 15))[2:]
                            self.texture_data += "0x" + str(g) + str(b) + '0' + str(r)
                            #self.texture_data += hex(int(pixel * 15))[2:]
                            
                        counter += 1
                        
                        if (counter == (new_size * new_size * 4) - 1):
                            self.texture_data += '};'
                        elif (counter % 4 == 3):
                            self.texture_data += ','
                        
                    self.texture_data += '\n\n'
                
                index = self.textures.index(n.image.filepath)
                
                #we make a scrolling texture if the word 'scroll' is anywhere in the name
                if (n.image.filepath.find('scroll') == -1):
                    shader_id = 100
                else:
                    shader_id = 101
                    
                
                
                #we only take the first item as the texture size
                image_size = n.image.size[0]
                #print(image_size)
                
                #image_size = 32
                
                
                
                vertices += str(shader_id) + ", " + str(index) + ", "
                
                
                u1 = (mesh.uv_layers[0].data[tri.loops[0]].uv[0]) * image_size
                v1 = (mesh.uv_layers[0].data[tri.loops[0]].uv[1]) * image_size
                
                u2 = (mesh.uv_layers[0].data[tri.loops[1]].uv[0]) * image_size
                v2 = (mesh.uv_layers[0].data[tri.loops[1]].uv[1]) * image_size
                
                u3 = (mesh.uv_layers[0].data[tri.loops[2]].uv[0]) * image_size
                v3 = (mesh.uv_layers[0].data[tri.loops[2]].uv[1]) * image_size
                
                    
                    
                
                
                #finally perform conversion into hex
                u1 = hex(int(u1))
                v1 = hex(int(v1))
                
                u2 = hex(int(u2))
                v2 = hex(int(v2))
                
                u3 = hex(int(u3))
                v3 = hex(int(v3))
                
                vertices += "0x" + u1[2:].zfill(2) + v1[2:].zfill(2) + ", 0x" + u2[2:].zfill(2) + v2[2:].zfill(2) + ", 0x" + u3[2:].zfill(2) + v3[2:].zfill(2)
                
            
            #otherwise we export using colors    
            else:

                #print(colors)
                #Get per vertex color from top default color layer
                
                #print(colors.data[tri.vertices[0]].color[3]) #alpha channel
                if colors:
                    
                    r1 = hex(int((colors.data[tri.loops[0]].color[0]) * 15))
                    g1 = hex(int((colors.data[tri.loops[0]].color[1]) * 15))
                    b1 = hex(int((colors.data[tri.loops[0]].color[2]) * 15))
                    
                    r2 = hex(int((colors.data[tri.loops[1]].color[0]) * 15))
                    g2 = hex(int((colors.data[tri.loops[1]].color[1]) * 15))
                    b2 = hex(int((colors.data[tri.loops[1]].color[2]) * 15))
                    
                    r3 = hex(int((colors.data[tri.loops[2]].color[0]) * 15))
                    g3 = hex(int((colors.data[tri.loops[2]].color[1]) * 15))
                    b3 = hex(int((colors.data[tri.loops[2]].color[2]) * 15))
                    

                                    
                    #Shader ID
                    #if all the colors are the same, we can use shader id 1 for simple flat shading
                    if (r1 == r2 == r3 and g1 == g2 == g3 and b1 == b2 == b3):
                        if glow:
                            shader_id = 11
                        else:
                            shader_id = 1
                    else:
                        if glow:
                            shader_id = 12
                        else:    
                            shader_id = 2
                    
                #if colors don't exist, use rgb interpolation    
                else:
                    shader_id = 2
                    
                    r1 = hex(15)
                    g1 = hex(0)
                    b1 = hex(0)
                    r2 = hex(0)
                    g2 = hex(15)
                    b2 = hex(0)
                    r3 = hex(0)
                    g3 = hex(0)
                    b3 = hex(15)
                    
                    
                vertices += str(shader_id) + ", 0, "
                
                vertices += "0x" + g1[2:] + b1[2:] + "0" + r1[2:] + ", 0x" + g2[2:] + b2[2:] + "0" + r2[2:] + ", 0x" + g3[2:] + b3[2:] + "0" + r3[2:]
            
            
            
            vertices += "}\n"
            triangles += 1
            
            #Abort if we hit the maximum number of triangles
            if triangles == max_triangles:
                break;
            
        
        output = "#define " + modelname.upper() + " " + str(triangles) + "\n"
        
        #Determine whether to store list in flash or ram (Chunks are always stored in flash)
        if (is_chunk == True or store_ram == False):
            output += "const "
        
        output += "struct "
        
        #decide whether to use 16 or 32 bit fixed point range
        if (reduced_range == True):
            output += "triangle_16 "
        else:
            output += "triangle_32 "
        
        output += modelname +"[" + modelname.upper() + "] = {\n" + vertices + "};"
        
        return output



    def find_repeat_chunk(self, lod, x, y):
        #go through list of meshes and see if there are any repeat chunks which can be used
        for mesh in bpy.data.meshes:
            if (mesh.name.startswith('repeat_lod' + str(lod))):
                #print(mesh.name[12:])
                pos = mesh.name.find('_', 12)
                xmin = int(mesh.name[12:pos])
                
                pos2 = mesh.name.find('_', pos + 1)
                xmax = int(mesh.name[pos+1:mesh.name.find('_', pos2)])
                
                pos3 = mesh.name.find('_', pos2 + 1)
                ymin = int(mesh.name[pos2+1:mesh.name.find('_', pos3)])
                
                pos4 = mesh.name.find('_', pos3 + 1)
                ymax = int(mesh.name[pos3+1:])
                
                #check if the given chunk is within the boundaries of the repeat chunk
                if (x >= xmin and x <= xmax and y >= ymin and y <=ymax):
                    return mesh.name
                
        return False
    
    
    
    def export_chunks(self, context, lod, max_triangles):
        
        world_size_x = context.scene.world_size_x
        world_size_y = context.scene.world_size_y
        
        repeat_chunk_list = []

        chunk_decl = f'const struct chunk lod{lod}_chunks[WORLD_SIZE_X][WORLD_SIZE_Y]'

        decl_output = f'extern {chunk_decl};\n'
        
        chunk_output = f'{chunk_decl} = {{\n'

        triangle_output = ''

        
        for x in range(world_size_x):
            
            chunk_output += '{'
            
            for y in range(world_size_y):
                
                meshname = 'lod' + str(lod) + '_' + str(x) + '_' + str(y)
                
                modelname = 'lod' + str(lod) + '_chunk_triangles_' + str(x) + '_' + str(y)
                
                #see if a chunk with the given name already exists in the model collection
                if meshname in bpy.data.meshes:
                    mesh = bpy.data.meshes[meshname]
                    
                    
                    
                    triangle_output += self.generate_vertices(context, mesh, modelname, max_triangles, True) + "\n"


                    if y == world_size_y - 1:
                        chunk_output += '{' + modelname.upper() + ', 0, ' + modelname + '}\n'
                    else:
                        chunk_output += '{' + modelname.upper() + ', 0, ' + modelname + '}, \n'
                        
                else:
                    #try to find a repeat chunk which can be used
                    repeat_chunk = self.find_repeat_chunk(lod, x, y)
                    
                    #if a chunk cannot be found, we create an empty one in its place (0 triangles)
                    if (repeat_chunk == False):
                        #print('Chunk not found:' + meshname)
                        if y == world_size_y - 1:
                            chunk_output += '{0, 0, NULL}\n'
                        else:
                            chunk_output += '{0, 0, NULL}, \n'
                    else:
                        #print('Using repeat chunk: ' + repeat_chunk + ' for chunk: lod' + str(lod) + '_' + str(x) + '_' + str(y))
                        
                        mesh = bpy.data.meshes[repeat_chunk]
                        
                        #see if repeat chunk has already been dumped out
                        
                        if (repeat_chunk not in repeat_chunk_list):
                            triangle_output += self.generate_vertices(context, mesh, repeat_chunk, max_triangles, True) + "\n"
                            repeat_chunk_list.append(repeat_chunk)
                        
                        
                        if y == world_size_y - 1:
                            chunk_output += '{' + repeat_chunk.upper() + ', 0, ' + repeat_chunk + '}\n'
                        else:
                            chunk_output += '{' + repeat_chunk.upper() + ', 0, ' + repeat_chunk + '}, \n'
                            
                        
            if x == world_size_x - 1:
                chunk_output += '}'
            else:
                chunk_output += '},'
                    
        
        chunk_output = triangle_output + chunk_output + '};'
        
        
        return decl_output, chunk_output
    
        
        
    def generate_textures(self):

        texture_count = len(self.textures)
        var_decl = f'struct texture chunk_texture_list[{texture_count}]'

        textures_decl = f'extern {var_decl};'

        #if there are no textures to be added, exit
        if (len(self.texture_list) == 0):
            #print('no textures to be added')
            return textures_decl, f'{var_decl} = {{}};'
        

        self.texture_list = f'{var_decl} = {{' + self.texture_list + '};'
        
        #print(self.texture_data)
        #print(self.texture_list)
        
        output = self.texture_data + self.texture_list
        
        return textures_decl, output
    
    
    
    def generate_lights(self, context):
        
        world_size_x = context.scene.world_size_x
        world_size_y = context.scene.world_size_y
        
        fixed_point_factor = context.scene.fixed_point_factor
        
        var_decl = f'const struct chunk_lighting chunk_lights[{world_size_x}][{world_size_y}]'

        decl_output = f'extern {var_decl};\n'
        chunk_lights = f'{var_decl} = {{'
        
        chunk_light_data = ''
        
        for x in range(world_size_x):
            
            chunk_lights += '{'
            
            for y in range(world_size_y):
                #see if there are any lights in a given chunk
                light_count = 0
                light_data = ''
                for object in bpy.context.scene.objects:
                    if object.type == 'LIGHT':
                        #create boundaries in which a chunk would have a light
                        min_x = (x * 10) - 5
                        min_y = -((y * 10) - 5)
                        max_x = (x * 10) + 5
                        max_y = -((y * 10) + 5)
                        
                        #if light is within boundary of chunk, add it to the lighting list for that chunk
                        if (object.location[0] >= min_x and object.location[0] < max_x and object.location[1] <= min_y and object.location[1] > max_y):
                            #convert to int
                            new_x = int((object.location[0] * fixed_point_factor) - (world_size_x * 10 * fixed_point_factor / 2) + (5 * fixed_point_factor))
                            new_y = int(object.location[2] * fixed_point_factor)
                            new_z = int(-(object.location[1] * fixed_point_factor) - (world_size_y * 10 * fixed_point_factor / 2) + (5 * fixed_point_factor))
                            
                            if (light_count != 0):
                                light_data += ',\n'
                                
                            light_data += '{' + str(new_x) + ',' + str(new_y) + ',' + str(new_z) + '}'
                            #print(object.location)
                            
                            light_count += 1
                            #print(x)
                            #print(y)
                        
                #write out light information for that chunk
                if (light_count > 0):
                    chunk_light_data += 'const struct light lights_' + str(x) + '_' + str(y) + '[' + str(light_count) + '] = {' + light_data + '};\n'
                    chunk_lights += '{' + str(light_count) + ', ' + 'lights_' + str(x) + '_' + str(y) + '}'
                else:
                    chunk_lights += '{0, NULL}'
                    
                #add separator
                if (y == (world_size_y - 1)):
                    chunk_lights += '}'
                else:
                    chunk_lights += ','
                    
            
            if (x == (world_size_x - 1)):
                chunk_lights += '};'
            else:
                chunk_lights += ',\n'
        
        output = chunk_light_data + chunk_lights
        #print(chunk_light_data)
        #print(chunk_lights)
        return decl_output, output
            
        

def menu_func(self, context):
    self.layout.operator(Pico3dExport.bl_idname)

def register():
    bpy.utils.register_class(Pico3dExport)
    bpy.utils.register_class(Pico3dPanel)
    
    for (prop_name, prop_value) in PROPS:
        setattr(bpy.types.Scene, prop_name, prop_value)
        
    for (prop_name, prop_value) in PROPS_ADVANCED:
        setattr(bpy.types.Scene, prop_name, prop_value)
        
    #bpy.types.VIEW3D_MT_object.append(menu_func)  # Adds the new operator to an existing menu.

def unregister():
    
    for (prop_name, _) in PROPS:
        delattr(bpy.types.Scene, prop_name)
    bpy.utils.unregister_class(Pico3dExport)

    for (prop_name, _) in PROPS_ADVANCED:
        delattr(bpy.types.Scene, prop_name)
        
    bpy.utils.unregister_class(Pico3dExport)
    bpy.utils.unregister_class(Pico3dPanel)

# This allows you to run the script directly from Blender's Text editor
# to test the add-on without having to install it.
if __name__ == "__main__":
    register()
