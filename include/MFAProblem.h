////////////////////////////////////////////////////////////////////////////////
//    MFAToolkit: Software for running flux balance analysis on stoichiometric models
//    Software developer: Christopher Henry (chenry@mcs.anl.gov), MCS Division, Argonne National Laboratory
//    Copyright (C) 2007  Argonne National Laboratory/University of Chicago. All Rights Reserved.
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//    For more information on MFAToolkit, see <http://bionet.mcs.anl.gov/index.php/Model_analysis>.
////////////////////////////////////////////////////////////////////////////////

#ifndef MFAPROBLEM_H
#define MFAPROBLEM_H

struct LinEquation;
struct MFAVariable;
struct OptSolutionData;
struct FileConstraints;
struct SavedBounds;

class MFAProblem{
private:
	bool Max;
	int Solver;
	int ProbType;	
	int NumLocalSolve;
	int ProblemIndex;
	bool ProblemLoaded;

	bool RelaxIntegerVariables;
	bool LoadedRelaxation;
	bool UseTightBounds;
	bool MakeAllDrainsSimultaneously;

	Data* SourceDatabase;
	LinEquation* ObjFunct;
	vector<LinEquation*> Constraints;
	map<string, int> ConstraintIndexMap;
	vector<MFAVariable*> Variables;
	vector<OptSolutionData*> Solutions;
	vector<ProblemState*> ProblemStates;
	int MFAProblemClockIndex;

	double CurrentOptimum;
	LinEquation* MinFluxConstraint;
	LinEquation* ObjectiveConstraint;
	LinEquation* ExometaboliteObjective;
	LinEquation* MetaboliteObjective;

	vector<vector<MFAVariable*> > MetDrains;
	vector<vector<MFAVariable*> > ExoDrains;
public:
	MFAProblem();
	~MFAProblem();
	
	//Input functions
	void SetMax();
	void SetMin();
	void SetSolver(const char* InSolver);
	void SetNumLocalSolve(int InNumLocalSolve);
	void ResetIndecies();
	void ClearSolutions(int Start = 0, int End = -1, bool DeleteThem = true);
	int AddVariable(MFAVariable* InVariable);
	int AddConstraint(LinEquation* InConstraint);
	void AddObjective(LinEquation* InObjective);
	void AddOptimizeVariableObjective(MFAVariable* InVariable, bool InMax);
	void ClearObjective(bool DeleteThem = true);
	void ClearConstraints(bool DeleteThem = true);
	void ClearVariables(bool DeleteThem = true);
	void DetermineProbType();
	LinEquation* AddSumObjective(int VarType, bool Quadratic, bool Append, double Coeff, bool ForeignOnly,LinEquation* InObjective = NULL);
	void AddMassBalanceConstraints(Data* InData,bool DilutionConstraints = false);
	LinEquation* AddSumConstraint(int VarType, bool Quadratic, double Coeff, double RHS, int EqualityType);
	LinEquation* AddUseSolutionConst(OptSolutionData* SolutionData, vector<int> VariableTypes, OptimizationParameter* InParameters);
	void EnforceIntergerSolution(OptSolutionData* SolutionData, vector<int> VariableTypes, bool ForeignOnly, bool RefreshSolver);
	void RelaxSolutionEnforcement(vector<int> VariableTypes, bool ForeignOnly, bool RefreshSolver);
	LinEquation* MakeObjectiveConstraint(double Value, int Equality = EQUAL);
	int BuildMFAProblem(Data* InData, OptimizationParameter*& InParameters);
	int BuildDualMFAProblem(MFAProblem* InProblem, Data* InData, OptimizationParameter*& InParameters);
	int ApplyInputBounds(FileBounds* InBounds, Data* InData, bool ApplyToMinMax = false);
	int ApplyInputConstraints(ConstraintsToAdd* AddConstraints, Data* InData);
	int ModifyInputConstraints(ConstraintsToModify* ModConstraints, Data* InData);
	void InputSolution(OptSolutionData* InSolution);
	LinEquation* CreateUseVariableConstraint(MFAVariable* InVariable,OptimizationParameter*& InParameters);
	LinEquation* CreateUseVariablePositiveConstraint(MFAVariable* InVariable,OptimizationParameter*& InParameters);
	LinEquation* CreateGibbsEnergyConstraint(Reaction* InReaction, OptimizationParameter*& InParameters);
	LinEquation* CreateReactionErrorConstraint(Reaction* InReaction, OptimizationParameter*& InParameters);
	void CreateSpeciesGibbsEnergyConstraint(Species* InSpecies, OptimizationParameter*& InParameters);
	LinEquation* ConvertStringToObjective(string ObjString, Data* InData,bool ReplaceObjective = true);
	void RemoveConstraint(int ConstraintIndex, bool DeleteConstraint = true);
	void RelaxConstraint(int ConstraintIndex);
	int SaveState();
	void ClearState(int InState);
	void LoadState(int InState, bool Bounds, bool Constraints, bool Objective, bool SaveSolutions, bool Parameters);
	void ResetVariableMarks(bool InMark);
	void ResetConstraintMarks(bool InMark);

	//Output functions
	bool FMax();
	int FSolver();
	int FNumConstraints();
	int FNumSolutions();
	int FNumVariables();
	MFAVariable* GetVariable(int InIndex);
	OptSolutionData* GetSolution(int InIndex);
	LinEquation* GetConstraint(int InIndex);
	LinEquation* GetObjective();
	bool FProblemLoaded();
	string fluxToString();
	
	//Solver interaction
	int LoadSolver(bool PrintFromSolver = true);
	OptSolutionData* RunSolver(bool SaveSolution,bool InInputSolution,bool WriteProblem);
	int LoadConstToSolver(int ConstraintNumber);
	int LoadAllConstToSolver() ;
	int LoadAllVariables();
	int LoadObjective();
	int LoadVariable(int InIndex);
	int ResetSolver();
	int UpdateLoadSolver();
	SavedBounds* saveBounds();
	int loadBounds(SavedBounds* inBounds,bool loadProblem = true);
	int loadChangedBoundsIntoSolver(SavedBounds* inBounds);
	int loadMedia(string media, Data* inData,bool loadIntoSolver = true);
	int clearOldMedia(OptimizationParameter* InParameters);

	//Analysis functions
	map<int , vector<double> , std::less<int> >* CalcConcMeanIonicStrength();
	void ProcessSolution(OptSolutionData* InSolution);
	int FindTightBounds(Data* InData,OptimizationParameter*& InParameters, bool SaveSolution, bool UseSpecifiedSearchTypes = false);
	int CatalogueFluxLoops(Data* InData,OptimizationParameter*& InParameters);
	int FindTightBounds(Data* InData,OptimizationParameter*& InParameters,string Note);
	int AlternativeSolutionExploration(OptimizationParameter* InParameters,string Filename,OptSolutionData* InitialSolution = NULL,bool clearmarks = true);
	int RecursiveMILP(Data* InData,OptimizationParameter*& InParameters, vector<int> VariableTypes,bool PrintSolutions);
	vector<OptSolutionData*> RecursiveMILP(OptimizationParameter* InParameters, string ProblemNote,bool ForeignOnly,vector<int> VariableTypes,double MinSolution,int ClockIndex,LinEquation* OriginalObjective);
	MFAVariable* CreateOrGetDrainVariable(Species* CurrentSpecies, int compartment,bool reversed = false,int type = DRAIN_FLUX);
	vector<MFAVariable*>* CreateMetaboliteVariables(Data* InData, string InMetaboliteList);
	int PerturbMinSolution(Data* InData, OptimizationParameter* InParameters,OptSolutionData*& CurrentSolution,map<MFAVariable*,vector<double>,std::less<MFAVariable*> >* CharacteristicFluxes,bool Subsample);
	int CalculateCharacteristicFlux(Data* InData, OptimizationParameter* InParameters,OptSolutionData*& CurrentSolution);
	int ComputeOptimalDeadends(Data* InData);
	int CheckIndividualMetaboliteProduction(Data* InData, OptimizationParameter* InParameters, string InMetaboliteList, bool DoFindTightBounds, bool MinimizeForeignReactions, string Note, bool SubProblem);
	vector<vector<MFAVariable*>*>* RecursiveObjectiveReduction(MFAVariable* InVariable,double MinObjective,double MaxObjective,LinEquation* MinDeviationObjective,OptSolutionData* PreviousSolution,int depth,vector<bool> blacklist);
	int AnalyzeMetaboliteInteractions(Data* InData, OptimizationParameter* InParameters,OptSolutionData*& CurrentSolution, bool consumption, bool production);
	int FitFluxVector(Data* InData, OptimizationParameter* InParameters,OptSolutionData*& CurrentSolution);
	int MinimizeFluxDeviation(vector<Reaction*> reactions, vector<double> fluxes,OptSolutionData*& CurrentSolution);
	int ReactionAdditionTesting(Data* InData, OptimizationParameter* InParameters,OptSolutionData*& CurrentSolution,LinEquation* NewObjective,vector<MFAVariable*> TestVariables,bool PrintResults);
	int ReduceObjective(Data* InData, OptimizationParameter* InParameters,OptSolutionData*& CurrentSolution);
	int SteadyStateCommunityModeling(Data* InData, OptimizationParameter* InParameters);
	OptSolutionData* ComputeMinimalDeviationFluxSolution(OptSolutionData* CurrentSolution);
	int RunDeletionExperiments(Data* InData, OptimizationParameter* InParameters,bool GapfillPhenosim = false,OptSolutionData* CurrentSolution = NULL);
	int RunMediaExperiments(Data* InData, OptimizationParameter* InParameters, double WildTypeObjective, bool DoOptimizeSingleObjective, bool DoFindTightBounds, bool MinimizeForeignReactions, bool OptimizeMetaboliteProduction);
	int DetermineMinimalFeasibleMedia(Data* InData,OptimizationParameter* InParameters,OptSolutionData*& CurrentSolution,bool augment_only = false);
	int OptimizeSingleObjective(Data* InData, OptimizationParameter* InParameters, string InObjective, bool FindTightBounds, bool MinimizeForeignReactions, double &ObjectiveValue, string Note);
	int OptimizeSingleObjective(Data* InData, OptimizationParameter* InParameters, bool FindTightBounds, bool MinimizeForeignReactions, double &ObjectiveValue, string Note, bool SubProblem);
	int CheckPotentialConstraints(Data* InData, OptimizationParameter* InParameters, double &ObjectiveValue, string Note);
	int OptimizeIndividualForeignReactions(Data* InData, OptimizationParameter* InParameters, bool FindTightBounds, bool OptimizeMetaboliteProduction);
	int FindSpecificExtremePathways(Data* InData, OptimizationParameter* InParameters);
	int FluxCouplingAnalysis(Data* InData, OptimizationParameter* InParameters, bool DoFindTightBounds, string &InNote, bool SubProblem);
	int ExploreSplittingRatios(Data* InData, OptimizationParameter* InParameters, bool FindTightBounds, bool MinimizeForeignReactions);
	int MILPCoessentialityAnalysis(Data* InData, OptimizationParameter* InParameters, bool DoFindTightBounds, string &InNote, bool SubProblem);	
	int RecursiveMILPStudy(Data* InData, OptimizationParameter* InParameters, bool DoFindTightBounds);
	int IdentifyReactionLoops(Data* InData, OptimizationParameter* InParameters);
	int LoadBiomassDrainReactions(Data* InData, OptimizationParameter* InParameters);
	int LoadGapFillingReactions(Data* InData, OptimizationParameter* InParameters);
	int MetabolomicsSensitivityAnalysis(Data* InData,OptSolutionData*& CurrentSolution);
	int CreateMetabolomicsVariablesConstraints(Data* InData, string PeakData, bool ExometabolomicData);
	int GapFilling(Data* InData, OptimizationParameter* InParameters,OptSolutionData*& CurrentSolution);
	int BinaryReactionDeactivationSearch(vector<MFAVariable*> variables,vector<double> lower_bounds,vector<double> upper_bounds);
	vector<MFAVariable*> BiomassSensitivityAnalysis(OptSolutionData*& CurrentSolution,OptimizationParameter* InParameters);
	int ReactionSensitivityAnalysis(Data* InData,OptSolutionData*& CurrentSolution,OptimizationParameter* InParameters);
	int RunImplementedGapfillingSolution(Data* InData, OptimizationParameter* InParameters,OptSolutionData*& CurrentSolution);
	int CompleteGapFilling(Data* InData, OptimizationParameter* InParameters,bool fastgapfill = false);
	int CalculateGapfillCoefficients(Data* InData,OptimizationParameter* InParameters,map<string,Reaction*,std::less<string> > InactiveVar,map<MFAVariable*,double,std::less<MFAVariable*> >& VariableCoefficients,bool fastgapfill = false);
	int GapGeneration(Data* InData, OptimizationParameter* InParameters);
	int SolutionReconciliation(Data* InData, OptimizationParameter* InParameters);
	string MediaSensitivityExperiment(Data* InData, OptimizationParameter* InParameters, vector<MFAVariable*> CurrentKO, vector<MFAVariable*> NonessentialMedia);
	int FitGIMME(Data* InData);
	int SoftConstraint(Data* InData);
	int FitGeneActivtyState(Data* InData);
	int GenerateMinimalReactionLists(Data* InData);
	int ParseRegExp(OptimizationParameter* InParameters, Data* InData, string Expression);
	int AddRegulatoryConstraints(OptimizationParameter* InParameters, Data* InData);
	void AddVariableToRegulationConstraint(LinEquation* InEquation,double Coefficient,string VariableName,Data* InData,OptimizationParameter* InParameters);
	int CalculateFluxSensitivity(Data* InData,vector<MFAVariable*> variables,double objective);
	double optimizeVariable(MFAVariable* currentVariable,bool maximize);
	int PrepProteinEnabledFBAFormulation(Data* InData, OptimizationParameter* InParameters);
	int SteadyStateProteinFBA(Data* InData, OptimizationParameter* InParameters);
	int DynamicFBA(Data* InData, OptimizationParameter* InParameters);
	
	int AddPROMConstraints(Data* InData, OptimizationParameter* InParameters,OptSolutionData*& CurrentSolution);
	int LoadAdditionalReactions(Data* InData,OptimizationParameter* InParameters);
	int BuildCoreProblem(Data* InData,OptimizationParameter*& InParameters);
	int AddMassBalanceAtomConstraint(const char* ID, Data* InData);
	int AddDeltaGofFormationConstraint(Data* InData);
	int AddUptakeLimitConstraints();
	int FluxBalanceAnalysisMasterPipeline(Data* InData, OptimizationParameter* InParameters);
	int QuantitativeModelOptimization(Data* InData, OptimizationParameter* InParameters);
	vector<MFAVariable*> SolveGapfillingProblem(int currentround,OptSolutionData*& CurrentSolution,string label,OptimizationParameter* InParameters,string OutputFilename);

	//FBA extension studies
	int CombinatorialKO(int maxDeletions,Data* InData, bool reactions);

	//File IO Functions
	void PrintProblemReport(double SingleObjective,OptimizationParameter* InParameters, string InNote);
	int LoadTightBounds(Data* InData, bool SetBoundToTightBounds);
	void SaveTightBounds();
	void PrintSolutions(int StartIndex, int EndIndex,bool tightbounds = false);
	void PrintVariableKey(string InFilename = "",bool override = false);
	void WriteLPFile();
	void WriteMFALog();
};

struct RegLogicNode {
	int Logic;
	int Level;
	vector<string> Object;
	vector<RegLogicNode*> LogicNodes;
	vector<bool> LogicOn;
	vector<bool> ObjectOn;
};

#endif
