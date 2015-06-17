# <pep8-80 compliant>
bl_info = {
    "name": "MESH EXPORT",
    "author": "Dmitry Kuzmin",
    "version": (2, 0, 0),
    "blender": (2, 58, 0),
    "location": "File > Import-Export",
    "description": "Exports some shit",
    "warning": "",
    "wiki_url": "http://wiki.blender.org/index.php/Extensions:2.6/Py/"
                "Scripts/Import-Export/Wavefront_OBJ",
    "support": 'OFFICIAL',
    "category": "Import-Export"}

import bpy
from bpy_extras.io_utils import ExportHelper


class ExportMesh(bpy.types.Operator, ExportHelper):
    bl_idname = "killmeplease.mesh"
    bl_label = "Convert"
    bl_options = {'PRESET'}

    filename_ext = ".mesh"

    def execute(self, context):

        tex = []

        file = open(self.filepath, 'w')

        bpy.ops.object.mode_set(mode='OBJECT')

        obj = context.object
        if not obj or obj.type != 'MESH':
            raise NameError("Cannot export: object %s is not a mesh" % obj)

        #for face in obj.data.tessfaces:
        #    for u, v in obj.data.tessface_uv_textures.active.data[face.index].uv:
        #        tex.append(u)
        #        tex.append(v)

        #bpy.ops.object.mode_set(mode='EDIT')
        uv = obj.data.tessface_uv_textures.active

        #if not uv:
        #    raise NameError("NO UV FOUND")
        def veckey2d(v):
            return round(v[0], 4), round(v[1], 4)

        uv_unique_count = no_unique_count = 0
        faceuv = len(obj.data.uv_textures) > 0

        """
        if faceuv:
            uv_texture = obj.data.uv_textures.active.data[:]
            uv_layer = obj.data.uv_layers.active.data[:]
            face_index_pairs = [(face, index) for index, face in enumerate(obj.data.polygons)]
            uv = f_index = uv_index = uv_key = uv_val = uv_ls = None
            uv_face_mapping = [None] * len(face_index_pairs)

            uv_dict = {}
            uv_get = uv_dict.get
            for f, f_index in face_index_pairs:
                uv_ls = uv_face_mapping[f_index] = []
                for uv_index, l_index in enumerate(f.loop_indices):
                    uv = uv_layer[l_index].uv
                    uv_key = veckey2d(uv)
                    uv_val = uv_get(uv_key)
                    if uv_val is None:
                        uv_val = uv_dict[uv_key] = uv_unique_count
                        file.write('%.6f %.6f\n' % uv[:])
                        uv_unique_count += 1
                    uv_ls.append(uv_val)

            del uv_dict, uv, f_index, uv_index, uv_ls, uv_get, uv_key, uv_val
        """
        uvs = []
        uv_layer = obj.data.uv_layers.active.data

        for poly in obj.data.polygons:
            #file.write("Polygon %i\n" % poly.index)
            for li in poly.loop_indices:
                vi = obj.data.loops[li].vertex_index
                uv = uv_layer[li].uv
                #file.write("    Loop index %i (Vertex %i) - UV %f %f\n" % (li, vi, uv.x, uv.y))
                uvs.append(uv)
            #for face in obj.data.polygons:
             #   for vert, loop in zip(face.vertices, face.loop_indices):
              #      for item in obj.data.uv_layers.active.data[loop].uv:#uv
               #         file.write(repr(item))#('%f %f\n' % (item[0], item[1]))
        #else:
        #    raise NameError("NO UV TEXTURES FOUND")

        #file.write(repr(uv))
        bpy.ops.object.mode_set(mode='OBJECT')
        file.write(str(len(obj.data.vertices)) + '\n')

        for vert in obj.data.vertices:
            file.write('%f %f %f %f %f %f\n' % (
                vert.co[0], vert.co[1], vert.co[2], vert.normal[0], vert.normal[1], vert.normal[2]))

        """i = 0
        while i < len(tex):
            file.write('%f %f\n' % (tex[i], tex[i + 1]))
            i += 2"""

        indices = []
        for face in obj.data.polygons:
            indices.append(face.vertices[0])
            indices.append(face.vertices[1])
            indices.append(face.vertices[2])



        #for uv in uvs:
        #    file.write('%f %f\n' % (uv.x, uv.y))

        file.write('{}\n'.format(len(indices)))

        i = 0

        while i < len(indices):
            file.write('%i %i %i %f %f %f %f %f %f\n' % (indices[i], indices[i + 1], indices[i + 2], uvs[i].x,uvs[i].y,uvs[i+1].x,uvs[i+1].y,uvs[i+2].x,uvs[i+2].y))
            i += 3

        file.close()
        return {'FINISHED'}


def menu_func_export(self, context):
    self.layout.operator(ExportMesh.bl_idname, text="To mesh (.mesh)")


def register():
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_module(__name__)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()