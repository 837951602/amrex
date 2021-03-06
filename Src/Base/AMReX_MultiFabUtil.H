#ifndef AMREX_MultiFabUtil_H_
#define AMREX_MultiFabUtil_H_

#include <AMReX_BaseFabUtility.H>
#include <AMReX_MultiFab.H>
#include <AMReX_iMultiFab.H>
#include <AMReX_LayoutData.H>
#include <AMReX_Array.H>
#include <AMReX_Vector.H>

namespace amrex
{
    //! Average nodal-based MultiFab onto cell-centered MultiFab.
    void average_node_to_cellcenter (MultiFab& cc, int dcomp,
                                     const MultiFab& nd, int scomp,
                                     int ncomp, int ngrow = 0);

    //! Average edge-based MultiFab onto cell-centered MultiFab. This fills in
    //! ngrow ghost cells in the cell-centered MultiFab. Both cell centered and
    //! edge centered MultiFabs need to have ngrow ghost values
    void average_edge_to_cellcenter (MultiFab& cc, int dcomp,
                                     const Vector<const MultiFab*>& edge,
                                     int ngrow = 0);

    //! Average face-based MultiFab onto cell-centered MultiFab.
    void average_face_to_cellcenter (MultiFab& cc, int dcomp,
                                     const Vector<const MultiFab*>& fc,
                                     int ngrow = 0);
    void average_face_to_cellcenter (MultiFab& cc, int dcomp,
                                     const Array<const MultiFab*,AMREX_SPACEDIM>& fc,
                                     int ngrow = 0);
    void average_face_to_cellcenter (MultiFab& cc,
                                     const Vector<const MultiFab*>& fc,
                                     const Geometry& geom);
    void average_face_to_cellcenter (MultiFab& cc,
                                     const Array<const MultiFab*,AMREX_SPACEDIM>& fc,
                                     const Geometry& geom);

    //! Average cell-centered MultiFab onto face-based MultiFab.
    void average_cellcenter_to_face (const Vector<MultiFab*>& fc,
                                     const MultiFab& cc,
                                     const Geometry& geom);
    void average_cellcenter_to_face (const Array<MultiFab*,AMREX_SPACEDIM>& fc,
                                     const MultiFab& cc,
                                     const Geometry& geom);

    //! Average fine face-based MultiFab onto crse face-based MultiFab.
    void average_down_faces (const Vector<const MultiFab*>& fine,
                             const Vector<MultiFab*>& crse,
                             const IntVect& ratio,
                             int ngcrse = 0);
    void average_down_faces (const Vector<const MultiFab*>& fine,
                             const Vector<MultiFab*>& crse,
                             int ratio,
                             int ngcrse = 0);
    void average_down_faces (const Array<const MultiFab*,AMREX_SPACEDIM>& fine,
                             const Array<MultiFab*,AMREX_SPACEDIM>& crse,
                             const IntVect& ratio,
                             int ngcrse = 0);
    void average_down_faces (const Array<const MultiFab*,AMREX_SPACEDIM>& fine,
                             const Array<MultiFab*,AMREX_SPACEDIM>& crse,
                             int ratio,
                             int ngcrse = 0);

    //! Average fine edge-based MultiFab onto crse edge-based MultiFab. This
    //! routine assumes that the crse BoxArray is a coarsened version of the
    //! fine BoxArray.
    void average_down_edges (const Vector<const MultiFab*>& fine,
                             const Vector<MultiFab*>& crse,
                             const IntVect& ratio,
                             int ngcrse = 0);

    //! Average fine node-based MultiFab onto crse node-centered MultiFab. This
    //! routine assumes that the crse BoxArray is a coarsened version of the
    //! fine BoxArray.
    void average_down_nodal (const MultiFab& S_fine,
                             MultiFab& S_crse,
                             const IntVect& ratio,
                             int ngcrse = 0);

    //! Average fine cell-based MultiFab onto crse cell-centered MultiFab using
    //! volume-weighting. This routine DOES NOT assume that the crse BoxArray is
    //! a coarsened version of the fine BoxArray.
    void average_down (const MultiFab& S_fine, MultiFab& S_crse,
                       const Geometry& fine_geom, const Geometry& crse_geom,
                       int scomp, int ncomp, const IntVect& ratio);
    void average_down (const MultiFab& S_fine, MultiFab& S_crse,
                       const Geometry& fine_geom, const Geometry& crse_geom,
                       int scomp, int ncomp, const int ratio);

    //! Average MultiFab onto crse MultiFab without volume weighting. This
    //! routine DOES NOT assume that the crse BoxArray is a coarsened version of
    //! the fine BoxArray. Work for both cell-centered and nodal MultiFabs.
    void average_down (const MultiFab& S_fine, MultiFab& S_crse,
                       int scomp, int ncomp, const IntVect& ratio);
    void average_down (const MultiFab& S_fine, MultiFab& S_crse,
                       int scomp, int ncomp, int ratio);

    //! Add a coarsened version of the data contained in the S_fine MultiFab to
    //! S_crse, including ghost cells.
    void sum_fine_to_coarse (const MultiFab& S_Fine, MultiFab& S_crse,
                             int scomp, int ncomp,
                             const IntVect& ratio,
                             const Geometry& cgeom, const Geometry& fgeom);

    //! Output state data for a single zone
    void print_state (const MultiFab& mf, const IntVect& cell, const int n=-1);

    //! Write each fab individually
    void writeFabs (const MultiFab& mf, const std::string& name);
    void writeFabs (const MultiFab& mf, int comp, int ncomp, const std::string& name);

    //! Convert iMultiFab to MultiFab
    MultiFab ToMultiFab (const iMultiFab& imf);
    //! Convert iMultiFab to long
    FabArray<BaseFab<long> > ToLongMultiFab (const iMultiFab& imf);

    //! Extract a slice from the given cell-centered MultiFab at coordinate
    //! "coord" along direction "dir".
    std::unique_ptr<MultiFab> get_slice_data(int dir, Real coord,
                                             const MultiFab& cc,
                                             const Geometry& geom, int fstart, int ncomp,
                                             bool interpolate=false);

    //! Is it safe to have these two MultiFabs in the same MFiter?
    //! Ture means safe; false means maybe.
    inline bool isMFIterSafe (const MultiFab& x, const MultiFab& y) {
        return x.DistributionMap() == y.DistributionMap()
            && BoxArray::SameRefs(x.boxArray(), y.boxArray());
    }

    //! Return an iMultiFab that has the same BoxArray and DistributionMapping
    //! as the coarse MultiFab cmf. Cells covered by the coarsened fine grids
    //! are set to one, whereas other cells are set to zero.
    iMultiFab makeFineMask (const MultiFab& cmf, const BoxArray& fba, const IntVect& ratio);

    //! Computes divergence of face-data stored in the umac MultiFab. It is
    //! assumed that umac has at least 1 ghost cell and ghost cells have been
    //! filled.
    void computeDivergence (MultiFab& divu, const Array<MultiFab const*,AMREX_SPACEDIM>& umac,
                            const Geometry& geom);


    //! General collection of MultiFab utilities
    namespace MFUtil {

        //! Template class defining a copy operation where source and target are
        //! grown by ng. Template parameter T must be either MultiFab or
        //! iMultiFab
        template<typename T>
            struct SymmetricGhost {
                static void copy (T & target, const T & source, int nc, int ng) {
                    target.copy(source, 0, 0, nc, ng, ng);
                }
            };


        //! Template class defining a copy operation where only the target is
        //! grown by ng. Template parameter T must be either MultiFab or
        //! iMultiFab
        template<typename T> // T can be either MultiFab or iMultiFab
            struct AsymmetricGhost {
                static void copy (T & target, const T & source, int nc, int ng) {
                    target.copy(source, 0, 0, nc, 0, ng);
                }
            };


        //! Duplicate class T (which must be either of type MultiFab or
        //! iMultiFab) using a new BoxArray and DistributionMapping. Data from
        //! the source (mf_in) is copied using a ParallelCopy (ghost cells are
        //! treated based on the Cpy class (which is either SymmetricGhost or
        //! AsymmetricGhost)).
        template<typename T, template<typename> class Cpy>
            T duplicate(const BoxArray & ba, const DistributionMapping & dm, const T & mf_in) {

            int ng = mf_in.nGrow();
            int nc = mf_in.nComp();

            T mf_out(ba, dm, nc, ng);

            Cpy<T>::copy(mf_out, mf_in, nc, ng);

            return mf_out;
        }


        //! Duplicate class T (which must be either of type MultiFab or
        //! iMultiFab) using a new BoxArray and DistributionMapping. Data from
        //! the source (mf_in) is copied using a ParallelCopy (ghost cells are
        //! treated based on the Cpy class (which is either SymmetricGhost or
        //! AsymmetricGhost)). Following the copy operation, a FillBoundary
        //! operation is called.
        template<typename T, template<typename> class Cpy>
            T duplicate(const BoxArray & ba, const DistributionMapping & dm, const T & mf_in,
                        const Periodicity & periodicity) {

            T mf_out = duplicate<T, Cpy>(ba, dm, mf_in);
            mf_out.FillBoundary(periodicity);

            return mf_out;
        }


        //! Convert class T_src (which must either be of type MultiFab or
        //! iMultiFab) to type T_dest (which must also be of type MultiFab or
        //! iMultiFab). This function is useful for convering iMultiFab MultiFab
        //! or vice-versa.
        template<typename T_src, typename T_dest>
            std::unique_ptr<T_dest> convert(const T_src & mf_in) {

            const BoxArray & ba            = mf_in.boxArray();
            const DistributionMapping & dm = mf_in.DistributionMap();
            int ncomp                = mf_in.nComp();
            int ngrow                = mf_in.nGrow();

            std::unique_ptr<T_dest> mf_out(new T_dest(ba, dm, ncomp, ngrow));

#ifdef _OPENMP
#pragma omp parallel
#endif
            for(MFIter mfi(mf_in, true); mfi.isValid(); ++ mfi) {
                const IArrayBox & in_tile  =      mf_in[mfi];
                FArrayBox & out_tile = (* mf_out)[mfi];

                for(BoxIterator bit(mfi.growntilebox()); bit.ok(); ++ bit) {

                    for(int i = 0; i < ncomp; i ++)
                        out_tile(bit(), i) = in_tile(bit(), i);

                }
            }

            return mf_out;
        }
    }
}


#endif
