#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>


//#include "src/OpenMesh/Core/IO/MeshIO.hh"
//#include "OpenMesh/Core/IO/MeshIO.hh" //?


#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/IO/Options.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
//typedef
using MyMesh = OpenMesh::PolyMesh_ArrayKernelT<>;


struct float3 { float x, y, z; };
struct int3 { int i0, i1, i2; };
struct Mesh {
    std::vector<float3> positions;
    std::vector<int3> triangles;
};
struct AABB {
    float3 min;
    float3 max;
};
struct AABB_Flag {
    AABB box;
    bool inside_flag;
};
struct Input {
    Mesh mesh;
    std::vector<AABB_Flag> input_data;
};
/*struct Output {
    std::vector<Mesh> output_data;
};*/
struct Output {
    Mesh mesh;
};

// implement these methods
void read(Input& input)
{
    // read test data from test_data files
    // mesh                      - mesh.obj
    // axis-Aligned Bounding Box - aabb*.txt
  
    MyMesh mesh;

    auto test = OpenMesh::IO::read_mesh(mesh, "../bunny.obj");
    if (!test)
    {
        std::cerr << "read error\n";
        exit(1);
    }
    else
    {
        std::cout << "Succes\n";
        
        
        if (mesh.is_triangles()) std::cout << "mesh consist of triangles";
        else std::cout << "No triangles!!";

        for (auto vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi)
        {   
            auto point = mesh.point(*vi);

            float3 pnt;
            pnt.x = point[0];
            pnt.y = point[1];
            pnt.z = point[2];
       //     if (pnt.x < 0)
            {
                input.mesh.positions.emplace_back(std::move(pnt));
          //      std::cout << pnt.x << " " << pnt.y << " " << pnt.z << "\n";
            }
                 
        }
    }

   // auto test_write = OpenMesh::IO::write_mesh(mesh, "../edited_bunny.obj");
   // mesh.vertex();
}

void write(Output& output) 
{
    // save meshes from output.output_data to separate files
    // file format - "mesh_result_{idx}.obj"
  //  for (auto vi = output.output_data.begin(); vi != output.output_data.end(); ++vi)
    MyMesh mesh;
    MyMesh::VertexHandle vhandle;
    std::vector<MyMesh::VertexHandle> face_vhandles;
    for (auto vi = output.mesh.positions.begin(); vi != output.mesh.positions.end(); ++vi)
    {
        float3 pnt;
        pnt = (*vi);

        mesh.add_vertex(MyMesh::Point(pnt.x , pnt.y, pnt.z ));

    }
    for (auto vi = mesh.vertices_begin(); vi+3 != mesh.vertices_end(); ++vi)
    {
       // auto point = mesh.point(*vi);

        mesh.add_face(*vi, *(vi + 1), *(vi + 2));
 //       mesh.faces_begin
    }

    OpenMesh::IO::Options OptionRead(OpenMesh::IO::Options::VertexTexCoord);

    if (!OptionRead.check(OpenMesh::IO::Options::VertexNormal))
    {
        std::cout << "Normals are absent\n";
        mesh.request_face_normals();
        mesh.update_normals();
        mesh.release_face_normals();
    }
    else
    {
        std::cout << "Normals are present";
    }

    auto test_write = OpenMesh::IO::write_mesh(mesh, "../edited_bunny_2.obj");
    std::cout << "writed new obj\n";
 
}

void intersect(const Mesh& inputMesh, const AABB_Flag& inputAABBFlag, Mesh& outputMesh)
{
    // implement intersection operation using only one current thread, processing each triangle one by one sequentially
    // print runtime to console
    
}
void intersect(const Mesh& inputMesh, Mesh& outputMesh)
{
    // implement intersection operation using only one current thread, processing each triangle one by one sequentially
    // print runtime to console
    outputMesh = inputMesh;
}
void intersectFast(const Mesh& inputMesh, const AABB_Flag& inputAABBFlag, Mesh& outputMesh) 
{
    // implement intersection operation.
    // Should work as fast as possible, using any tricks for speedup 
    // (additional structures, fast search in space, some fast filter unmatched triangles, multithreading, etc)
    // print runtime to console

}

void intersect(const Input& input, Output& output)
{
   // output.output_data.resize(input.input_data.size());
    int outputIdx = 0;
    intersect(input.mesh, output.mesh);

   /* for (auto& inputData : input.input_data)
    {
     //   intersect(input.mesh, inputData, output.output_data[outputIdx++]);
        //intersectFast(input.mesh, inputData, output.output_data[outputIdx++]);
    }*/
}

void main()
{
    Input input;
    read(input);

    Output output;
    intersect(input, output);

    write(output);
  
}
